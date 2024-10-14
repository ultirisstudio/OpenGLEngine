#pragma once

#include <unordered_map>

#include <OpenGLEngine/Core/UUID.h>
#include <OpenGLEngine/ECS/Registry.h>

namespace OpenGLEngine
{
	class Entity;
	class Registry;

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
		bool isOnRuntime() { return m_OnRuntime; }

		int m_LightsCount;

		void ClearEntities();

		Registry* GetRegistry() { return m_Registry.get(); }

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry->GetRegistry().view<Components...>();
		}
	private:
		EntityMap m_EntityMap;
		std::unique_ptr<Registry> m_Registry;

		bool m_OnRuntime;
	};
}