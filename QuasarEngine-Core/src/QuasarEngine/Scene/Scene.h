#pragma once

#include <unordered_map>

#include <QuasarEngine/Scene/Camera.h>
#include <QuasarEngine/Tools/Chronometer.h>
#include <QuasarEngine/Scene/Skybox.h>
#include <QuasarEngine/ECS/Registry.h>

namespace QuasarEngine
{
	class Entity;

	using EntityMap = std::unordered_map<UUID, entt::entity>;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		void DestroyEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		void Update(double deltaTime);
		void UpdateRuntime(double deltaTime);

		void OnRuntimeStart();
		void OnRuntimeStop();

		int m_LightsCount;

		bool isOnRuntime() { return m_OnRuntime; }

		void ClearEntities();

		Skybox& getSkybox() { return *m_Skybox; }

		Registry* GetRegistry() { return m_Registry.get(); }

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry->GetRegistry().view<Components...>();
		}
	private:
		EntityMap m_EntityMap;
		std::unique_ptr<Registry> m_Registry;

		std::unique_ptr<Skybox> m_Skybox;

		bool m_OnRuntime;
	};
}