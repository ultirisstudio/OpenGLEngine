#include "qepch.h"
#include <QuasarEngine/Scene/Scene.h>
#include <QuasarEngine/Entity/Entity.h>

#include "QuasarEngine/Core/Application.h"
#include <GLFW/glfw3.h>

#include <QuasarEngine/Core/UUID.h>
#include <QuasarEngine/Entity/Components/LightComponent.h>
#include <QuasarEngine/Entity/Components/TransformComponent.h>
#include <QuasarEngine/Entity/Components/CameraComponent.h>
#include <QuasarEngine/Entity/Components/ScriptComponent.h>
#include <QuasarEngine/Entity/Components/HierarchyComponent.h>
#include <QuasarEngine/Entity/Components/Physics/RigidBodyComponent.h>
#include <QuasarEngine/Entity/Components/Gameplay/CharacterControllerComponent.h>

#include <QuasarEngine/Core/Input.h>

//#include <QuasarEngine/Scripting/ScriptEngine.h>
#include <QuasarEngine/Physic/PhysicEngine.h>

namespace QuasarEngine
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
		m_Registry = std::make_unique<Registry>();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry->CreateEntity(), m_Registry.get() };
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
				return Entity{ entity, m_Registry.get()};
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), m_Registry.get()};

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

		PhysicEngine::Update(deltaTime);

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
		if (!m_ActiveCamera)
		{
			std::cout << "No active camera set!" << std::endl;
			return;
		}

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

	void Scene::ResizeActiveCamera(float width, float height)
	{
		if (!m_ActiveCamera)
			return;

		m_ActiveCamera->OnResize(width, height);
		Renderer::SetViewport(0, 0, width, height);
	}

	/*EntityMap* Scene::getEntities()
	{
		return &m_EntityMap;
	}*/

	void Scene::ClearEntities()
	{
		//m_EntityMap.clear();
	}
}