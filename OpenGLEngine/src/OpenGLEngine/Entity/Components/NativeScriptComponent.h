#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Entity/ScriptableEntity.h>

namespace OpenGLEngine
{
	class NativeScriptComponent : public Component
	{
	private:
		std::string script_name;
	public:
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}

		char* getScriptName() { return script_name.data(); }
	};
}