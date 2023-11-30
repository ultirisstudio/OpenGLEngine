#pragma once

#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#endif

#include <Windows.h>

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Entity/ScriptableEntity.h>

#include <OpenGLEngine/Script/Script.h>

namespace OpenGLEngine
{
	using _get_script_creator = script_creator(*)(size_t);
	using _get_script_names = LPSAFEARRAY(*)(void);

	class NativeScriptComponent : public Component
	{
	private:
		HINSTANCE hinstance{ nullptr };
		_get_script_creator fun_script_creator{ nullptr };
		_get_script_names fun_script_names{ nullptr };

		std::string m_ScriptName;

		std::vector<std::string> m_LoadedScriptNames;

		script_ptr m_Script = nullptr;
	public:
		NativeScriptComponent();

		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* InstantiateScript();
		//ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		/*template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}*/

		void Bind();

		char* getScriptName() { return m_ScriptName.data(); }
		void setScriptName(const char* scriptName) { m_ScriptName = scriptName; }

		std::vector<std::string>& getLoadedScriptNames() { return m_LoadedScriptNames; }
	};
}