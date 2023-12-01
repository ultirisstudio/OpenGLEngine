#pragma once

#include <unordered_map>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Scene/EditorCamera.h>

namespace OpenGLEngine
{
	using EntityMap = std::unordered_map<std::string, Entity>;

	class Scene
	{
	public:
		Scene();
		Scene(const std::string& name);

		Entity* CreateEntity(const std::string& name = std::string());
		Entity* CreateEntityWithUUID(const std::string& uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity* FindEntityByName(std::string name);
		Entity* GetEntityByUUID(std::string uuid);

		std::vector<Entity*> GetEntityVector();

		void Update(double deltaTime);
		void UpdateRuntime(double deltaTime);

		void OnScenePlay();
		void OnSceneStop();

		void ResizeEditorCamera(float width, float height);
		void ResizeActiveCamera(float width, float height);

		EditorCamera& getEditorCamera() { return *m_EditorCamera; }
		Camera* getActiveCamera() { return m_ActiveCamera; }

		void setActiveCamera(Camera* camera) { m_ActiveCamera = camera; }

		std::vector<Entity*> getDrawEntities();

		Entity* m_SelectedEntity;

		const std::string& getName() { return m_Name; }
		const std::string& getPath() { return m_Path; }

		void setName(const std::string& name) { m_Name = name; }
		void setPath(const std::string& path) { m_Path = path; }
	private:
		EntityMap m_EntityMap;

		std::unique_ptr<EditorCamera> m_EditorCamera;
		Camera* m_ActiveCamera;

		std::string m_Name;
		std::string m_Path;

		bool m_OnRuntime;

		friend class SceneSerializer;
	public:
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