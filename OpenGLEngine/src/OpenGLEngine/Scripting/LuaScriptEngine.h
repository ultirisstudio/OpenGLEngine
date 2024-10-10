#pragma once

#include <sol/sol.hpp>

namespace OpenGLEngine
{
	class Scene;

	class LuaScriptEngine
	{
	private:
		Scene* m_Scene;
		bool bMainLoaded;

	public:
		LuaScriptEngine(Scene* scene);
		~LuaScriptEngine() = default;

		bool loadMainScript(sol::state& lua);
		void Update();
		void Render();
	};
}