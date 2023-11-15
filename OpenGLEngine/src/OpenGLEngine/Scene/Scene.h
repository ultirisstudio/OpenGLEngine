#pragma once

#include <unordered_map>

#include "OpenGLEngine/Entity/Entity.h"

namespace OpenGLEngine
{
	using EntityMap = std::unordered_map<std::string, Entity>;

	class Scene
	{
	public:
		Scene() = default;

		Entity* CreateEntity(const std::string& name = std::string());
		Entity* CreateEntityWithUUID(const std::string& uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity* FindEntityByName(std::string name);
		Entity* GetEntityByUUID(std::string uuid);

		std::vector<Entity*> GetEntityVector();

		void OnUpdate(double deltaTime);
		void RenderScene();

		void OnScenePlay();
		void OnSceneStop();
	private:
		EntityMap m_EntityMap;
	private:
		template<typename Component>
		std::vector<Entity*> View()
		{
			std::vector<Entity*> entities;
			for (auto it = m_EntityMap.begin(); it != m_EntityMap.end(); it++)
			{
				if (it->second.HasComponent<Component>())
				{
					entities.push_back(&it->second);
				}
			}

			return entities;
		}
	};
}