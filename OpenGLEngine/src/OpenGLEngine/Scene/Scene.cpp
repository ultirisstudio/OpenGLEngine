#include "depch.h"
#include <OpenGLEngine/Scene/Scene.h>

#include "OpenGLEngine/Core/Application.h"
#include <GLFW/glfw3.h>

#include <OpenGLEngine/Core/UUID.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/ScriptComponent.h>
#include <OpenGLEngine/Entity/Components/Physics/RigidBodyComponent.h>
#include <OpenGLEngine/Entity/Components/Gameplay/CharacterControllerComponent.h>

#include <OpenGLEngine/Core/Input.h>

#include <OpenGLEngine/Scripting/ScriptEngine.h>
#include <OpenGLEngine/Physic/PhysicEngine.h>

namespace OpenGLEngine
{
	Scene::Scene() :
		m_Name("Untitled"),
		m_Path(""),
		m_ActiveCamera(nullptr),
		m_OnRuntime(false),
		m_LightsCount(0)
	{
		Init();
	}

	Scene::Scene(const std::string& name) : 
		m_Name(name),
		m_Path(""),
		m_ActiveCamera(nullptr),
		m_OnRuntime(false),
		m_LightsCount(0)
	{
		Init();
	}

	Scene::~Scene()
	{
		
	}

	void Scene::Init()
	{
		m_Skybox = std::make_unique<Skybox>();
	}

	Entity* Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity* Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		m_EntityMap[uuid] = Entity(name);
		Entity* entity = &m_EntityMap[uuid];
		entity->AddComponent<IDComponent>(uuid);
		entity->AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntityByUUID(UUID uuid)
	{
		auto it = m_EntityMap.find(uuid);
		if (it == m_EntityMap.end()) {
			std::cerr << "WARNING: Entity with UUID " << uuid << " not found in scene." << std::endl;
			return;
		}
		
		Entity* entity = &it->second;

		Entity* parent = GetEntityByUUID(entity->m_Parent);
		if (parent != nullptr) {
			auto childIt = std::find(parent->m_Children.begin(), parent->m_Children.end(), uuid);
			if (childIt != parent->m_Children.end()) {
				parent->m_Children.erase(childIt);
			}
		}

		entity->Destroy();
		m_EntityMap.erase(uuid);
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

	Entity* Scene::GetEntityByUUID(UUID uuid)
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

	void Scene::Update(double deltaTime)
	{
		if (m_OnRuntime)
			UpdateRuntime(deltaTime);
	}

	void Scene::UpdateRuntime(double deltaTime)
	{
		for (auto entity : View<ScriptComponent>())
		{
			ScriptEngine::OnUpdateEntity(*entity, deltaTime);
		}

		PhysicEngine::Update(deltaTime);

		for (Entity* entity : View<RigidBodyComponent>())
		{
			entity->GetComponent<RigidBodyComponent>().Update();
		}

		for (Entity* entity : View<CharacterControllerComponent>())
		{
			entity->GetComponent<CharacterControllerComponent>().Update();
		}
	}

	void Scene::OnRuntimeStart()
	{
		if (!m_ActiveCamera)
		{
			std::cout << "No active camera set!" << std::endl;
			return;
		}

		m_OnRuntime = true;

		ScriptEngine::OnRuntimeStart(this);

		for (auto entity : View<ScriptComponent>())
		{
			ScriptEngine::OnCreateEntity(*entity);
		}
	}

	void Scene::OnRuntimeStop()
	{
		m_OnRuntime = false;

		ScriptEngine::OnRuntimeStop();
	}

	void Scene::ResizeActiveCamera(float width, float height)
	{
		if (!m_ActiveCamera)
			return;

		m_ActiveCamera->OnResize(width, height);
		Renderer::SetViewport(0, 0, width, height);
	}

	EntityMap* Scene::getEntities()
	{
		return &m_EntityMap;
	}

	void Scene::ClearEntities()
	{
		m_EntityMap.clear();
	}
}