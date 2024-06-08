#include "SceneManager.h"

#include <filesystem>

#include <OpenGLEngine.h>

#include "SceneSerializer.h"

#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
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

	void SceneManager::AddGameObject(const std::string& file)
	{
		Renderer::LoadModel(file);
	}

	void SceneManager::AddCube()
	{
		Renderer::LoadModel("Assets/Models/cube.obj");
	}

	void SceneManager::AddSphere()
	{
		Renderer::LoadModel("Assets/Models/sphere.obj");
	}

	void SceneManager::AddUVSphere()
	{
		Entity* temp = m_Scene->CreateEntity("UV Sphere");

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;

		const float PI = 3.14159265359f;

		bool oddRow = false;

		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				vertices.push_back({ glm::vec3(xPos, yPos, zPos), glm::vec3(xPos, yPos, zPos), glm::vec2(xSegment, ySegment) });
			}
		}

		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow)
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}

		temp->AddComponent<MaterialComponent>();
		temp->AddComponent<MeshComponent>().GenerateMesh(vertices, indices, DrawMode::TRIANGLE_STRIP);
	}

	void SceneManager::AddPlane()
	{
		Renderer::LoadModel("Assets/Models/plane.obj");
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