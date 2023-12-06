#pragma once

#include <string>
#include <memory>

#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Tools/FileBrowser.h>

namespace OpenGLEngine
{
	enum DEFAULT_OBJECT_TYPE
	{
		CUBE,
		SPHERE,
		PLANE
	};

	class SceneManager
	{
	public:
		SceneManager();

		void update(double dt);

		Scene& getActiveScene() { return *m_Scene; }

		void AddGameObject(DEFAULT_OBJECT_TYPE type);
		void AddGameObject(const std::string& file);

		void SaveScene();
		void LoadScene();
		void LoadScene(std::string filePath);

		void createNewScene();

		void OpenExternalFile();
	private:
		std::unique_ptr<Scene> m_Scene;
		FileBrowser m_FileBrowser;
	};
}

