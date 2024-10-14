#include "qepch.h"
#include "LuaScriptEngine.h"

#include <iostream>

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Scene/Scene.h>
#include <QuasarEngine/Entity/Components/LuaScriptComponent.h>

namespace QuasarEngine
{
	LuaScriptEngine::LuaScriptEngine(Scene* scene) : m_Scene(scene), bMainLoaded(false)
	{
	}

	bool LuaScriptEngine::loadMainScript(sol::state& lua)
	{
		try
		{
			auto result = lua.safe_script_file("./Assets/Scripts/main.lua");
		}
		catch (const sol::error& err)
		{
			std::cerr << "Error when loading lua main script: " << err.what() << std::endl;
			return false;
		}

		sol::table main_lua = lua["main"];
		sol::optional<sol::table> bUpdateExists = main_lua[1];
		if (bUpdateExists == sol::nullopt)
		{
			std::cerr << "There is no update function in main.lua !" << std::endl;
			return false;
		}

		sol::table update_script = main_lua[1];
		sol::function update = update_script["update"];

		sol::optional<sol::table> bRenderExists = main_lua[2];
		if (bRenderExists == sol::nullopt)
		{
			std::cerr << "There is no render function in main.lua !" << std::endl;
			return false;
		}

		sol::table render_script = main_lua[2];
		sol::function render = render_script["render"];

		Entity mainLuaScript = m_Scene->CreateEntity("main_script");
		auto& luaScriptComponent = mainLuaScript.AddComponent<LuaScriptComponent>();
		luaScriptComponent.update = update;
		luaScriptComponent.render = render;

		bMainLoaded = true;
		return true;
	}

	void LuaScriptEngine::Update()
	{
		if (!bMainLoaded)
		{
			std::cerr << "Lua main script is not loaded !" << std::endl;
		}

		auto view = m_Scene->GetAllEntitiesWith<LuaScriptComponent>();
		for (const auto& e : view)
		{
			Entity entity{ e, m_Scene->GetRegistry()};
			if (entity.GetName() != "main_script")
				continue;

			auto& script = entity.GetComponent<LuaScriptComponent>();
			auto error = script.update(entity);
			if (!error.valid())
			{
				sol::error err = error;
				std::cerr << "Error when running the update script: " << err.what() << std::endl;
			}
		}
	}

	void LuaScriptEngine::Render()
	{
		if (!bMainLoaded)
		{
			std::cerr << "Lua main script is not loaded !" << std::endl;
		}

		auto view = m_Scene->GetAllEntitiesWith<LuaScriptComponent>();
		for (const auto& e : view)
		{
			Entity entity{ e, m_Scene->GetRegistry() };
			if (entity.GetName() != "main_script")
				continue;

			auto& script = entity.GetComponent<LuaScriptComponent>();
			auto error = script.render(entity);
			if (!error.valid())
			{
				sol::error err = error;
				std::cerr << "Error when running the render script: " << err.what() << std::endl;
			}
		}
	}
}