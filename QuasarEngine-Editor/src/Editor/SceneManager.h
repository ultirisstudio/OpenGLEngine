#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include "SceneObject.h"

namespace QuasarEngine
{
	class SceneManager
	{
	public:
		SceneManager(std::filesystem::path assetPath);

		void update(double dt);

		Scene& GetActiveScene() { return m_SceneObject->GetScene(); }
		SceneObject& GetSceneObject() { return *m_SceneObject; }

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
		std::unique_ptr<SceneObject> m_SceneObject;

		std::filesystem::path m_AssetPath;
	};
}

