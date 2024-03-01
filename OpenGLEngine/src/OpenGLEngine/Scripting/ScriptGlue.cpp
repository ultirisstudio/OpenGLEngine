#include "depch.h"
#include "ScriptGlue.h"

#include <mono/metadata/object.h>

#include <glm/glm.hpp>

#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Scripting/ScriptEngine.h>

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/ScriptComponent.h>
#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>

#include <OpenGLEngine/Tools/UUID.h>
#include <OpenGLEngine/Core/KeyCodes.h>
#include <OpenGLEngine/Core/Input.h>
#include <mono/metadata/reflection.h>

namespace OpenGLEngine
{
	template<typename... Component>
	struct ComponentGroup
	{

	};

	using AllComponents = ComponentGroup<TransformComponent, ScriptComponent, CameraComponent, LightComponent, MeshComponent, MaterialComponent, ModelComponent, TerrainComponent>;

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> m_EntityHasComponentFuncs;

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("OpenGLEngine.InternalCalls::" #Name, Name)

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		return m_EntityHasComponentFuncs.at(managedType)(*entity);
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		*outTranslation = entity->GetComponent<TransformComponent>().Position;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		entity->GetComponent<TransformComponent>().Position = *translation;
	}

	static void TransformComponent_GetRotation(UUID entityID, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		*outRotation = entity->GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		entity->GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID entityID, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		*outScale = entity->GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		entity->GetComponent<TransformComponent>().Scale = *scale;
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = "OpenGLEngine." + std::string(structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
				std::cout << "Failed to find managed type: " << managedTypename << std::endl;
			m_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(Entity_HasComponent);

		ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		ADD_INTERNAL_CALL(TransformComponent_SetRotation);

		ADD_INTERNAL_CALL(TransformComponent_GetScale);
		ADD_INTERNAL_CALL(TransformComponent_SetScale);

		ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}