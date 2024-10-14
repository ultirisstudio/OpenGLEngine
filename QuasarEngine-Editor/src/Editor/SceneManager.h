#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include <QuasarEngine/Scene/Scene.h>

#include "../Utils/FileBrowser.h"

namespace QuasarEngine
{
	class SceneManager
	{
	public:
		SceneManager(std::filesystem::path assetPath);

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
		std::filesystem::path m_AssetPath;
	};
}

