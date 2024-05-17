#include "SceneManager.h"

#include <filesystem>

#include <OpenGLEngine.h>

#include "SceneSerializer.h"

#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>

#include <OpenGLEngine/Physic/PhysicEngine.h>

namespace OpenGLEngine
{
	SceneManager::SceneManager()
	{
		m_Scene = std::make_unique<Scene>();
	}

	void SceneManager::update(double dt)
	{
		m_Scene->Update(dt);
	}

	void SceneManager::AddGameObject(DEFAULT_OBJECT_TYPE type)
	{
		Entity* temp = m_Scene->CreateEntity("temp");
		temp->AddComponent<TransformComponent>();

		switch (type)
		{
		case CUBE:
			temp->SetName("Cube");
			temp->AddComponent<ModelComponent>();
			temp->GetComponent<ModelComponent>().SetModel("Assets/Models/cube.obj");
			break;
		case SPHERE:
			temp->SetName("Sphere");
			temp->AddComponent<ModelComponent>();
			temp->GetComponent<ModelComponent>().SetModel("Assets/Models/sphere.obj");
			break;
		case PLANE:
			temp->SetName("Plane");
			temp->AddComponent<ModelComponent>();
			temp->GetComponent<ModelComponent>().SetModel("Assets/Models/plane.obj");
			break;
		}
	}

	void SceneManager::AddGameObject(const std::string& file)
	{
		const size_t slash = file.find_last_of("/\\");
		const std::string m_SelectedFile = file.substr(slash + 1);

		size_t lastindex = m_SelectedFile.find_last_of(".");
		const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

		Entity* temp = m_Scene->CreateEntity(m_FileName);
		temp->AddComponent<TransformComponent>();
		temp->AddComponent<ModelComponent>();
		temp->GetComponent<ModelComponent>().SetModel(file);
	}

	void SceneManager::SaveScene()
	{
		if (m_Scene->getPath() != "")
		{
			SceneSerializer serializer(*m_Scene);
			serializer.Serialize(m_Scene->getPath());
		}
		else
		{
			if (!m_FileBrowser.SaveFile())
				return;

			m_Scene->setName(m_FileBrowser.GetInfos().m_FileName);
			std::cout << m_FileBrowser.GetInfos().m_FilePath << std::endl;
			std::cout << m_FileBrowser.GetInfos().m_SelectedFile << std::endl;
			SceneSerializer serializer(*m_Scene);
			serializer.Serialize(m_FileBrowser.GetInfos().m_FilePath);
		}
	}

	void SceneManager::LoadScene()
	{
		if (!m_FileBrowser.OpenFile())
			return;

		PhysicEngine::Reload();

		m_Scene = std::make_unique<Scene>();
		SceneSerializer serializer(*m_Scene);
		serializer.Deserialize(m_FileBrowser.GetInfos().m_FilePath);

		for (Entity* entity : m_Scene->View<CameraComponent>())
		{
			m_Scene->setActiveCamera(&entity->GetComponent<CameraComponent>().GetCamera());
		}
	}

	void SceneManager::LoadScene(std::string filePath)
	{
		PhysicEngine::Reload();

		m_Scene = std::make_unique<Scene>();

		Renderer::BeginScene(*m_Scene);

		SceneSerializer serializer(*m_Scene);
		serializer.Deserialize(filePath);

		for (Entity* entity : m_Scene->View<CameraComponent>())
		{
			m_Scene->setActiveCamera(&entity->GetComponent<CameraComponent>().GetCamera());
		}
	}

	void SceneManager::ReloadScene(std::string filePath)
	{
		PhysicEngine::Reload();

		m_Scene->ClearEntities();

		SceneSerializer serializer(*m_Scene);
		serializer.DeserializeRuntime(filePath);

		for (Entity* entity : m_Scene->View<CameraComponent>())
		{
			m_Scene->setActiveCamera(&entity->GetComponent<CameraComponent>().GetCamera());
		}
	}

	void SceneManager::createNewScene()
	{
		m_Scene = std::make_unique<Scene>();
	}

	void SceneManager::OpenExternalFile()
	{
		if (!m_FileBrowser.OpenFile())
			return;

		std::filesystem::path sourceFile = m_FileBrowser.GetInfos().m_FilePath;
		std::filesystem::path targetParent = "Assets";
		auto target = targetParent / sourceFile.filename();

		try
		{
			std::filesystem::create_directories(targetParent);
			std::filesystem::copy_file(sourceFile, target, std::filesystem::copy_options::overwrite_existing);
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
		}
	}
}