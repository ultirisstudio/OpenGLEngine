#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Entity/ScriptableEntity.h>

//#include <OpenGLEngine/Script/Script.h>

namespace OpenGLEngine
{
	class NativeScriptComponent : public Component
	{
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
	};
}