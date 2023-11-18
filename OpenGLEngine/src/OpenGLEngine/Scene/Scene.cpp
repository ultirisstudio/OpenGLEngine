#include "depch.h"
#include <OpenGLEngine/Scene/Scene.h>

#include <OpenGLEngine/Tools/UUID.h>
#include <OpenGLEngine/Entity/Components/RenderComponent.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/NativeScriptComponent.h>

namespace OpenGLEngine
{
	Scene::Scene() : m_Name("Untitled"), m_Path("")
	{

	}

	Scene::Scene(const std::string& name) : m_Name(name), m_Path("")
	{
		
	}

	Entity* Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(uuid::generate_uuid_v4(), name);
	}

	Entity* Scene::CreateEntityWithUUID(const std::string& uuid, const std::string& name)
	{
		Entity entity = Entity(name, uuid);
		entity.AddComponent<TransformComponent>();
		m_EntityMap[entity.GetId()] = entity;
		return &m_EntityMap[entity.GetId()];
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetId());
	}

	Entity* Scene::FindEntityByName(std::string name)
	{
		for (auto it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
		{
			if (it->second.GetName() == name)
				return &it->second;
		}

		return nullptr;
	}

	Entity* Scene::GetEntityByUUID(std::string uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return &m_EntityMap.at(uuid);

		return nullptr;
	}

	std::vector<Entity*> Scene::GetEntityVector()
	{
		std::vector<Entity*> entities;
		for (auto it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
		{
			entities.push_back(&it->second);
		}

		return entities;
	}

	void Scene::OnUpdate(double deltaTime)
	{
		for (Entity* entity : View<NativeScriptComponent>()) {
			NativeScriptComponent& nsc = entity->GetComponent<NativeScriptComponent>();

			if (nsc.Instance == nullptr) {
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = *entity;
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(deltaTime);
		}
	}

	void Scene::RenderScene()
	{
		for (Entity* entity : View<RenderComponent>()) {
			entity->GetComponent<RenderComponent>().Draw();
		}
	}
	void Scene::OnScenePlay()
	{
		
	}

	void Scene::OnSceneStop()
	{
		
	}
}