#include "SceneManager.h"

#include <filesystem>

#include <QuasarEngine.h>

#include "SceneSerializer.h"
#include "Tools/Utils.h"

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/CameraComponent.h>
#include <QuasarEngine/Entity/Components/MeshComponent.h>
#include <QuasarEngine/Entity/Components/MaterialComponent.h>
#include <QuasarEngine/Entity/Components/MaterialComponent.h>
#include <QuasarEngine/Entity/Components/LightComponent.h>
#include <QuasarEngine/Entity/Components/MeshRendererComponent.h>

#include <QuasarEngine/Physic/PhysicEngine.h>

namespace QuasarEngine
{
	SceneManager::SceneManager(std::filesystem::path assetPath) : m_AssetPath(assetPath)
	{
		m_SceneObject = std::make_unique<SceneObject>();
	}

	void SceneManager::update(double dt)
	{
		
	}

	void SceneManager::AddGameObject(const std::string& file)
	{
		Renderer::LoadModel(file);
	}

	void SceneManager::AddGLTFGameObject(const std::string& file)
	{
		Renderer::LoadGLTFModel(file);
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
		Entity temp = m_SceneObject->GetScene().CreateEntity("UV Sphere");

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

		temp.AddComponent<MaterialComponent>();
		temp.AddComponent<MeshRendererComponent>();
		temp.AddComponent<MeshComponent>().GenerateMesh(vertices, indices, DrawMode::TRIANGLE_STRIP);
	}

	void SceneManager::AddPlane()
	{
		Renderer::LoadModel("Assets/Models/plane.obj");
	}

	void SceneManager::SaveScene()
	{
		if (m_SceneObject->GetPath() != "")
		{
			SceneSerializer serializer(*m_SceneObject, m_AssetPath);
			serializer.Serialize(m_SceneObject->GetPath());
		}
		else
		{
			std::optional<Utils::FileInfo> fileInfos = Utils::saveFile();
			if (fileInfos.has_value())
			{
				m_SceneObject->SetName(fileInfos.value().fileName);
				std::cout << fileInfos.value().filePath << std::endl;
				std::cout << fileInfos.value().selectedFile << std::endl;
				SceneSerializer serializer(*m_SceneObject, m_AssetPath);
				serializer.Serialize(fileInfos.value().filePath);
			}
		}
	}

	void SceneManager::LoadScene()
	{
		std::optional<Utils::FileInfo> fileInfos = Utils::openFile();
		if (fileInfos.has_value())
		{
			PhysicEngine::Reload();

			m_SceneObject->CreateScene();
			SceneSerializer serializer(*m_SceneObject, m_AssetPath);
			serializer.Deserialize(fileInfos.value().filePath);
		}
	}

	void SceneManager::LoadScene(std::string filePath)
	{
		PhysicEngine::Reload();

		m_SceneObject->CreateScene();

		Renderer::BeginScene(m_SceneObject->GetScene());

		SceneSerializer serializer(*m_SceneObject, m_AssetPath);
		serializer.Deserialize(filePath);
	}

	void SceneManager::ReloadScene(std::string filePath)
	{
		PhysicEngine::Reload();

		m_SceneObject->GetScene().ClearEntities();

		SceneSerializer serializer(*m_SceneObject, m_AssetPath);
		serializer.DeserializeRuntime(filePath);
	}

	void SceneManager::createNewScene()
	{
		m_SceneObject->CreateScene();
	}

	void SceneManager::OpenExternalFile()
	{
		std::optional<Utils::FileInfo> fileInfos = Utils::openFile();
		if (fileInfos.has_value())
		{
			std::filesystem::path sourceFile = fileInfos.value().filePath;
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
}