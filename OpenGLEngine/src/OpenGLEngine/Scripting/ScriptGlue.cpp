#include "depch.h"
#include "ScriptGlue.h"

#include <mono/metadata/object.h>

#include <glm/glm.hpp>

#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Scripting/ScriptEngine.h>

#include <OpenGLEngine/Perlin/PerlinManager.h>

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/IDComponent.h>
#include <OpenGLEngine/Entity/Components/ScriptComponent.h>
#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>
#include <OpenGLEngine/Entity/Components/RigidBodyComponent.h>

#include <reactphysics3d/reactphysics3d.h>

#include <OpenGLEngine/Core/UUID.h>
#include <OpenGLEngine/Core/KeyCodes.h>
#include <OpenGLEngine/Core/Input.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/appdomain.h>

namespace OpenGLEngine
{
	namespace Utils {

		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}

	}

	template<typename... Component>
	struct ComponentGroup
	{

	};

	using AllComponents = ComponentGroup<TransformComponent, ScriptComponent, CameraComponent, LightComponent, MeshComponent, MaterialComponent, ModelComponent, TerrainComponent, RigidBodyComponent>;

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> m_EntityHasComponentFuncs;
	static std::unordered_map<MonoType*, std::function<Component(Entity*)>> m_EntityAddComponentFuncs;

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("OpenGLEngine.InternalCalls::" #Name, Name)

	static void Debug_Log(MonoString* message)
	{
		std::string str = Utils::MonoStringToString(message);
		std::cout << str << std::endl;
	}

	static uint64_t Scene_CreateEntity(MonoString* name)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->CreateEntity(Utils::MonoStringToString(name));
		return entity->GetUUID();
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		return m_EntityHasComponentFuncs.at(managedType)(*entity);
	}

	static void Entity_AddComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		auto& c = m_EntityAddComponentFuncs.at(managedType)(entity);
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

	static void MeshComponent_GenerateMesh(UUID entityID, MonoArray* vertices, MonoArray* indices)
	{
		uint32_t verticesCount = mono_array_length(vertices);
		uint32_t indicesCount = mono_array_length(indices);

		//std::cout << "Vertices: " << verticesCount << " Indices: " << indicesCount << std::endl;

		std::vector<Vertex> vertexData;
		vertexData.reserve(verticesCount);
		for (uint32_t i = 0; i < verticesCount; i+=8)
		{
			Vertex vertex;
			vertex.position = glm::vec3(
				mono_array_get(vertices, float, i),
				mono_array_get(vertices, float, i+1),
				mono_array_get(vertices, float, i+2)
			);
			vertex.normal = glm::vec3(
				mono_array_get(vertices, float, i+3),
				mono_array_get(vertices, float, i+4),
				mono_array_get(vertices, float, i+5)
			);
			vertex.texCoord = glm::vec2(
				mono_array_get(vertices, float, i+6),
				mono_array_get(vertices, float, i+7)
			);
			vertexData.push_back(vertex);
		}

		std::vector<uint32_t> indexData;
		indexData.reserve(indicesCount);
		for (uint32_t i = 0; i < indicesCount; i++)
		{
			indexData.push_back(mono_array_get(indices, uint32_t, i));
		}

		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		entity->GetComponent<MeshComponent>().GenerateMesh(vertexData, indexData);
	}

	static uint64_t Perlin_Initialize()
	{
		uint64_t uuid = PerlinManager::AddPerlinNoise();
		return uuid;
	}

	static float Perlin_GetNoise(uint64_t id, float x, float z)
	{
		float result = PerlinManager::GetPerlinNoise(id, x, z);
		return result;
	}

	static float Perlin_GetMapHeight(uint64_t id, float x, float z)
	{
		float result = PerlinManager::GetMapHeight(id, x, z);
		return result;
	}

	static void ScriptComponent_SetScriptName(uint64_t entityID, MonoString* scriptName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity* entity = scene->GetEntityByUUID(entityID);
		entity->GetComponent<ScriptComponent>().m_Name = Utils::MonoStringToString(scriptName);

		ScriptEngine::OnCreateEntity(*entity);
	}

	static void RigidBody_ApplyLocalForceAtCenterOfMass(uint64_t entityID, glm::vec3 force)
	{
		//Scene* scene = ScriptEngine::GetSceneContext();
		//Entity* entity = scene->GetEntityByUUID(entityID);
		//entity->GetComponent<RigidBodyComponent>().GetRigidBody()->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(force.x, force.y, force.z));
		//entity->GetComponent<RigidBodyComponent>().GetRigidBody()->applyLocalTorque(reactphysics3d::Vector3(0.0f, 10.0f, 0.0f));
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
			m_EntityAddComponentFuncs[managedType] = [](Entity* entity) { return entity->AddComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		//RegisterComponent(AllComponents{});

		RegisterComponent<TransformComponent>();
		RegisterComponent<MeshComponent>();
		RegisterComponent<MaterialComponent>();
		RegisterComponent<ScriptComponent>();
		RegisterComponent<RigidBodyComponent>();
	}

	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(Debug_Log);

		ADD_INTERNAL_CALL(Scene_CreateEntity);

		ADD_INTERNAL_CALL(Entity_HasComponent);
		ADD_INTERNAL_CALL(Entity_AddComponent);

		ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		ADD_INTERNAL_CALL(TransformComponent_GetScale);
		ADD_INTERNAL_CALL(TransformComponent_SetScale);

		ADD_INTERNAL_CALL(MeshComponent_GenerateMesh);

		ADD_INTERNAL_CALL(Perlin_Initialize);
		ADD_INTERNAL_CALL(Perlin_GetNoise);
		ADD_INTERNAL_CALL(Perlin_GetMapHeight);

		ADD_INTERNAL_CALL(ScriptComponent_SetScriptName);

		ADD_INTERNAL_CALL(RigidBody_ApplyLocalForceAtCenterOfMass);

		ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}