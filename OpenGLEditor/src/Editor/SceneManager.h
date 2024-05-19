#pragma once

#include <string>
#include <memory>

#include <OpenGLEngine/Scene/Scene.h>

#include "../Utils/FileBrowser.h"

namespace OpenGLEngine
{
	enum DEFAULT_OBJECT_TYPE
	{
		CUBE,
		SPHERE,
		UV_SPHERE,
		PLANE
	};

	class SceneManager
	{
	public:
		SceneManager();

		void update(double dt);

		Scene& getActiveScene() { return *m_Scene; }

		void AddGameObject(const std::string& file);

		void AddCube();
		void AddSphere();
		void AddUVSphere();
		void AddPlane();

		void SaveScene();
		void LoadScene();
		void LoadScene(std::string filePath);

		void ReloadScene(std::string filePath);

		void createNewScene();

		void OpenExternalFile();
	private:
		std::unique_ptr<Scene> m_Scene;
		FileBrowser m_FileBrowser;
	};
}

