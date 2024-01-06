#include "SceneManager.h"

#include <filesystem>

#include <OpenGLEngine.h>
#include <OpenGLEngine/Scene/SceneSerializer.h>

#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>

namespace OpenGLEngine
{
	SceneManager::SceneManager()
	{
		m_Scene = std::make_unique<Scene>();

		/*Entity* temp = m_Scene->CreateEntity("dir_light");
		temp->AddComponent<TransformComponent>();
		temp->GetComponent<TransformComponent>().SetRotation(glm::vec3(-2.5f, -7.5f, -9.1f));
		temp->AddComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL);

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				for (int k = 0; k < 5; k++)
				{
					Entity* temp = m_Scene->CreateEntity("test_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k));
					temp->AddComponent<TransformComponent>(glm::vec3({ i * 6.0f, j * 6.0f, k * 6.0f }));
					temp->AddComponent<ModelComponent>("Assets\\Models\\BackPack.obj");
					temp->AddComponent<MaterialComponent>();
					temp->GetComponent<MaterialComponent>().GetMaterial().addVec3("ambient", glm::vec3({ 0.0f, 0.0f, 0.0f }));
					temp->GetComponent<MaterialComponent>().GetMaterial().addVec3("diffuse", glm::vec3({ 0.0f, 0.0f, 0.0f }));
					temp->GetComponent<MaterialComponent>().GetMaterial().addVec3("specular", glm::vec3({ 0.0f, 0.0f, 0.0f }));
					temp->GetComponent<MaterialComponent>().GetMaterial().addBoolean("diffuse", false);
					temp->GetComponent<MaterialComponent>().GetMaterial().addBoolean("specular", false);
					temp->GetComponent<MaterialComponent>().addTexture("diffuse", "Assets\\Textures\\1001_albedo.jpg");
					temp->GetComponent<MaterialComponent>().addTexture("specular", "Assets\\Textures\\1001_metallic.jpg");
					temp->GetComponent<MaterialComponent>().GetMaterial().addFloat("shininess", 32.0f);
				}
			}
		}*/
	}

	void SceneManager::update(double dt)
	{
		m_Scene->Update(dt);
	}

	void SceneManager::AddGameObject(DEFAULT_OBJECT_TYPE type)
	{
		Entity* temp = m_Scene->CreateEntity("temp");
		temp->AddComponent<TransformComponent>();
		temp->AddComponent<MaterialComponent>();
		temp->GetComponent<MaterialComponent>().InitializeMaterial();

		switch (type)
		{
		case CUBE:
			temp->SetName("Cube");
			temp->AddComponent<ModelComponent>("Assets/Models/cube.obj");
			break;
		case SPHERE:
			temp->SetName("Sphere");
			temp->AddComponent<ModelComponent>("Assets/Models/sphere.obj");
			break;
		case PLANE:
			temp->SetName("Plane");
			temp->AddComponent<ModelComponent>("Assets/Models/plane.obj");
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
		temp->AddComponent<ModelComponent>(file);
		temp->AddComponent<MaterialComponent>();
		temp->GetComponent<MaterialComponent>().InitializeMaterial();
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
		m_Scene = std::make_unique<Scene>();
		SceneSerializer serializer(*m_Scene);
		serializer.Deserialize(filePath);

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