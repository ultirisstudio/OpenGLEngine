#include "depch.h"
#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/ECS/Entity.h>

#include "OpenGLEngine/Core/Application.h"
#include <GLFW/glfw3.h>

#include <OpenGLEngine/Core/UUID.h>
#include <OpenGLEngine/ECS/Components/LightComponent.h>
#include <OpenGLEngine/ECS/Components/TransformComponent.h>
#include <OpenGLEngine/ECS/Components/CameraComponent.h>
#include <OpenGLEngine/ECS/Components/ScriptComponent.h>
#include <OpenGLEngine/ECS/Components/HierarchyComponent.h>
#include <OpenGLEngine/ECS/Components/Physics/RigidBodyComponent.h>
#include <OpenGLEngine/ECS/Components/Gameplay/CharacterControllerComponent.h>

#include <OpenGLEngine/Core/Input.h>

//#include <OpenGLEngine/Scripting/ScriptEngine.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>

namespace OpenGLEngine
{
	Scene::Scene() :
		m_OnRuntime(false),
		m_LightsCount(0)
	{
		m_Registry = std::make_unique<Registry>();
	}

	Scene::~Scene()
	{
		
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = {m_Registry->CreateEntity(), m_Registry.get()};
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<HierarchyComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry->DestroyEntity(entity);
	}

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = GetAllEntitiesWith<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, m_Registry.get() };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), m_Registry.get() };

		return {};
	}

	void Scene::Update(double deltaTime)
	{
		if (m_OnRuntime)
			UpdateRuntime(deltaTime);
	}

	void Scene::UpdateRuntime(double deltaTime)
	{
		/*for (auto e : m_Registry.view<ScriptComponent>())
		{
			Entity entity = { e, this };
			ScriptEngine::OnUpdateEntity(entity, deltaTime);
		}*/

		/*for (auto e : m_Registry.view<RigidBodyComponent>())
		{
			Entity entity = { e, this };
			entity.GetComponent<RigidBodyComponent>().Update();
		}

		for (auto e : m_Registry.view<CharacterControllerComponent>())
		{
			Entity entity = { e, this };
			entity.GetComponent<CharacterControllerComponent>().Update();
		}*/
	}

	void Scene::OnRuntimeStart()
	{
		/*if (!m_ActiveCamera)
		{
			std::cout << "No active camera set!" << std::endl;
			return;
		}*/

		m_OnRuntime = true;

		/*ScriptEngine::OnRuntimeStart(this);

		for (auto e : m_Registry.view<ScriptComponent>())
		{
			Entity entity = { e, this };
			ScriptEngine::OnCreateEntity(entity);
		}*/
	}

	void Scene::OnRuntimeStop()
	{
		m_OnRuntime = false;

		//ScriptEngine::OnRuntimeStop();
	}

	void Scene::ClearEntities()
	{
		m_EntityMap.clear();

		m_Registry->ClearRegistry();
	}
}