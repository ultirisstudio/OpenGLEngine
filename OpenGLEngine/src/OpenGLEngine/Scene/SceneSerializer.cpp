#include "depch.h"
#include "SceneSerializer.h"

#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/SkyboxComponent.h>
#include <OpenGLEngine/Entity/Components/RenderComponent.h>
#include <OpenGLEngine/Entity/Components/NativeScriptComponent.h>
#include <OpenGLEngine/Entity/Components/CameraComponent.h>

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}

namespace OpenGLEngine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity* entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity->GetName();
		out << YAML::Key << "ID" << YAML::Value << entity->GetId();

		out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

		if (entity->HasComponent<TransformComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "TransformComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& tc = entity->GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity->HasComponent<MaterialComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "MaterialComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mc = entity->GetComponent<MaterialComponent>();

			bool hasDiffuse = *mc.GetMaterial().getBoolean("diffuse");
			bool hasSpecular = *mc.GetMaterial().getBoolean("specular");

			out << YAML::Key << "hasDiffuse" << YAML::Value << hasDiffuse;
			out << YAML::Key << "hasSpecular" << YAML::Value << hasSpecular;

			out << YAML::Key << "ambient" << YAML::Value << *mc.GetMaterial().getVec3("ambient");

			if (hasDiffuse)
			{
				out << YAML::Key << "diffuse" << YAML::Value << mc.m_DiffuseTexture;
			}
			else
			{
				out << YAML::Key << "diffuse" << YAML::Value << *mc.GetMaterial().getVec3("diffuse");
			}

			if (hasSpecular)
			{
				out << YAML::Key << "specular" << YAML::Value << mc.m_SpecularTexture;
			}
			else
			{
				out << YAML::Key << "specular" << YAML::Value << *mc.GetMaterial().getVec3("specular");
			}

			out << YAML::Key << "shininess" << YAML::Value << *mc.GetMaterial().getFloat("shininess");

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity->HasComponent<ModelComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "ModelComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mc = entity->GetComponent<ModelComponent>();
			out << YAML::Key << "model" << YAML::Value << mc.m_ModelPath;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity->HasComponent<SkyboxComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "SkyboxComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mc = entity->GetComponent<ModelComponent>();
			out << YAML::Key << "skybox" << YAML::Value << true;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity->HasComponent<RenderComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "RenderComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mc = entity->GetComponent<ModelComponent>();
			out << YAML::Key << "render" << YAML::Value << true;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity->HasComponent<CameraComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "CameraComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& cc = entity->GetComponent<CameraComponent>();
			out << YAML::Key << "fov" << YAML::Value << cc.GetCamera().getFov();

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->getName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto& entity : m_Scene->GetEntityVector())
		{
			SerializeEntity(out, entity);
		};

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

		m_Scene->setPath(filepath);
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
		{
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		m_Scene->setName(sceneName);
		m_Scene->setPath(filepath);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				std::string name = entity["Entity"].as<std::string>();
				std::string uuid = entity["ID"].as<std::string>();

				//std::cout << "Deserializing entity with ID: " << uuid << ", Name: " << name << std::endl;

				Entity* deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto components = entity["Components"];
				if (components)
				{
					for (auto component : components)
					{
						auto transformComponent = component["TransformComponent"];
						if (transformComponent)
						{
							auto& tc = deserializedEntity->AddComponent<TransformComponent>();
							tc.Position = transformComponent["Position"].as<glm::vec3>();
							tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
							tc.Scale = transformComponent["Scale"].as<glm::vec3>();
						}

						auto modelComponent = component["ModelComponent"];
						if (modelComponent)
						{
							auto& mc = deserializedEntity->AddComponent<ModelComponent>(modelComponent["model"].as<std::string>());
						}

						auto materialComponent = component["MaterialComponent"];
						if (materialComponent)
						{
							auto& mc = deserializedEntity->AddComponent<MaterialComponent>();
							mc.InitializeMaterial();

							bool hasDiffuse = materialComponent["hasDiffuse"].as<bool>();
							bool hasSpecular = materialComponent["hasSpecular"].as<bool>();

							mc.GetMaterial().addVec3("ambient", materialComponent["ambient"].as<glm::vec3>());

							if (hasDiffuse)
							{
								mc.addTexture("diffuse", materialComponent["diffuse"].as<std::string>());
							}
							else
							{
								mc.GetMaterial().addVec3("diffuse", materialComponent["diffuse"].as<glm::vec3>());
							}

							if (hasSpecular)
							{
								mc.addTexture("specular", materialComponent["specular"].as<std::string>());
							}
							else
							{
								mc.GetMaterial().addVec3("specular", materialComponent["specular"].as<glm::vec3>());
							}

							mc.GetMaterial().addFloat("shininess", materialComponent["shininess"].as<float>());
						}

						auto skyboxComponent = component["SkyboxComponent"];
						if (skyboxComponent)
						{
							auto& sc = deserializedEntity->AddComponent<SkyboxComponent>();
						}

						auto renderComponent = component["RenderComponent"];
						if (renderComponent)
						{
							auto& rc = deserializedEntity->AddComponent<RenderComponent>();
							rc.GenerateShader();
						}

						auto cameraComponent = component["CameraComponent"];
						if (cameraComponent)
						{
							auto& cc = deserializedEntity->AddComponent<CameraComponent>();
							cc.GetCamera().setFov(cameraComponent["fov"].as<float>());
						}
					}
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		return false;
	}
}