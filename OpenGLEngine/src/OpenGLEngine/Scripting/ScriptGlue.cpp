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

	using AllComponents = ComponentGroup<TransformComponent, ScriptComponent, CameraComponent, LightComponent, MeshComponent, MaterialComponent, ModelComponent, TerrainComponent>;

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> m_EntityHasComponentFuncs;

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("OpenGLEngine.InternalCalls::" #Name, Name)

	static void Debug_Log(MonoString* message)
	{
		std::string str = Utils::MonoStringToString(message);
		std::cout << str << std::endl;
	}

	static MonoArray* Debug_ListTest()
	{
		MonoDomain* domain = ScriptEngine::GetCoreDomain();
		MonoClass* stringClass = mono_get_string_class();
		MonoArray* array = mono_array_new(domain, stringClass, 3);

		MonoString* str1 = mono_string_new(domain, "Hello");
		MonoString* str2 = mono_string_new(domain, "World");
		MonoString* str3 = mono_string_new(domain, "This");

		mono_array_set(array, MonoString*, 0, str1);
		mono_array_set(array, MonoString*, 1, str2);
		mono_array_set(array, MonoString*, 2, str3);
		return array;
	}

	static void Debug_SendArray(MonoArray* array)
	{
		MonoDomain* domain = ScriptEngine::GetCoreDomain();
		uint32_t length = mono_array_length(array);
		for (uint32_t i = 0; i < length; i++)
		{
			std::cout << mono_array_get(array, int, i) << std::endl;
		}
	}

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

	static void MeshComponent_GenerateMesh(UUID entityID, MonoArray* vertices, MonoArray* indices)
	{
		uint32_t verticesCount = mono_array_length(vertices);
		uint32_t indicesCount = mono_array_length(indices);

		std::cout << "Vertices: " << verticesCount << " Indices: " << indicesCount << std::endl;

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
		//RegisterComponent(AllComponents{});

		RegisterComponent<TransformComponent>();
		RegisterComponent<MeshComponent>();
	}

	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(Debug_Log);
		ADD_INTERNAL_CALL(Debug_ListTest);
		ADD_INTERNAL_CALL(Debug_SendArray);

		ADD_INTERNAL_CALL(Entity_HasComponent);

		ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		ADD_INTERNAL_CALL(TransformComponent_GetScale);
		ADD_INTERNAL_CALL(TransformComponent_SetScale);

		ADD_INTERNAL_CALL(MeshComponent_GenerateMesh);

		ADD_INTERNAL_CALL(Perlin_Initialize);
		ADD_INTERNAL_CALL(Perlin_GetNoise);

		ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}