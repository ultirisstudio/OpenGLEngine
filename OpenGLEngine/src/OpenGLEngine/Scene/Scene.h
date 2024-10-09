#pragma once

#include <unordered_map>

#include <OpenGLEngine/Scene/Camera.h>
#include <OpenGLEngine/Tools/Chronometer.h>
#include <OpenGLEngine/Scene/Skybox.h>

#include <entt.hpp>

namespace OpenGLEngine
{
	class Entity;

	using EntityMap = std::unordered_map<UUID, entt::entity>;

	class Scene
	{
	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		void Init();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		void DestroyEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		void Update(double deltaTime);
		void UpdateRuntime(double deltaTime);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void ResizeActiveCamera(float width, float height);

		Camera* getActiveCamera() { return m_ActiveCamera; }

		void setActiveCamera(Camera* camera) { m_ActiveCamera = camera; }

		//EntityMap* getEntities();

		int m_LightsCount;

		bool isOnRuntime() { return m_OnRuntime; }

		void ClearEntities();

		const std::string& getName() { return m_Name; }
		const std::string& getPath() { return m_Path; }

		Skybox& getSkybox() { return *m_Skybox; }

		void setName(const std::string& name) { m_Name = name; }
		void setPath(const std::string& path) { m_Path = path; }

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		EntityMap m_EntityMap;

		std::unique_ptr<Skybox> m_Skybox;
		Camera* m_ActiveCamera;

		std::string m_Name;
		std::string m_Path;

		bool m_OnRuntime;

		entt::registry m_Registry;

		friend class Entity;
		friend class SceneSerializer;
	};
}