#include "depch.h"
#include "SceneSerializer.h"

#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>

#include <OpenGLEngine/Core/Input.h>
#include <OpenGLEngine/Core/KeyCodes.h>


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

			bool hasAlbedo = *mc.GetMaterial().getBoolean("albedo");
			bool hasNormal = *mc.GetMaterial().getBoolean("normal");
			bool hasMetallic = *mc.GetMaterial().getBoolean("metallic");
			bool hasRoughness = *mc.GetMaterial().getBoolean("roughness");
			bool hasAO = *mc.GetMaterial().getBoolean("ao");

			out << YAML::Key << "hasAlbedo" << YAML::Value << hasAlbedo;
			out << YAML::Key << "hasNormal" << YAML::Value << hasNormal;
			out << YAML::Key << "hasMetallic" << YAML::Value << hasMetallic;
			out << YAML::Key << "hasRoughness" << YAML::Value << hasRoughness;
			out << YAML::Key << "hasAO" << YAML::Value << hasAO;

			out << YAML::Key << "albedo" << YAML::Value << *mc.GetMaterial().getVec3("albedo");
			out << YAML::Key << "metallic" << YAML::Value << *mc.GetMaterial().getFloat("metallic");
			out << YAML::Key << "roughness" << YAML::Value << *mc.GetMaterial().getFloat("roughness");
			out << YAML::Key << "ao" << YAML::Value << *mc.GetMaterial().getFloat("ao");

			if (hasAlbedo)
			{
				out << YAML::Key << "albedoMap" << YAML::Value << mc.m_AlbedoTexture;
			}

			if (hasNormal)
			{
				out << YAML::Key << "normalMap" << YAML::Value << mc.m_NormalTexture;
			}

			if (hasMetallic)
			{
				out << YAML::Key << "metallicMap" << YAML::Value << mc.m_MetallicTexture;
			}

			if (hasRoughness)
			{
				out << YAML::Key << "roughnessMap" << YAML::Value << mc.m_RoughnessTexture;
			}

			if (hasAO)
			{
				out << YAML::Key << "aoMap" << YAML::Value << mc.m_AOTexture;
			}

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

		if (entity->HasComponent<LightComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "LightComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& lc = entity->GetComponent<LightComponent>();

			if (lc.lightType == LightComponent::LightType::DIRECTIONAL)
			{
				out << YAML::Key << "lightType" << YAML::Value << "Directional";
				out << YAML::Key << "color" << YAML::Value << lc.dir_color;
			}
			else if (lc.lightType == LightComponent::LightType::POINT)
			{
				out << YAML::Key << "lightType" << YAML::Value << "Point";

				out << YAML::Key << "color" << YAML::Value << lc.point_color;
			}

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

				std::cout << "Deserializing entity with ID: " << uuid << ", Name: " << name << std::endl;

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

							bool hasAlbedo = materialComponent["hasAlbedo"].as<bool>();
							bool hasNormal = materialComponent["hasNormal"].as<bool>();
							bool hasMetallic = materialComponent["hasMetallic"].as<bool>();
							bool hasRoughness = materialComponent["hasRoughness"].as<bool>();
							bool hasAO = materialComponent["hasAO"].as<bool>();

							*mc.GetMaterial().getBoolean("albedo") = hasAlbedo;
							*mc.GetMaterial().getBoolean("normal") = hasNormal;
							*mc.GetMaterial().getBoolean("metallic") = hasMetallic;
							*mc.GetMaterial().getBoolean("roughness") = hasRoughness;
							*mc.GetMaterial().getBoolean("ao") = hasAO;

							*mc.GetMaterial().getVec3("albedo") = materialComponent["albedo"].as<glm::vec3>();
							*mc.GetMaterial().getFloat("metallic") = materialComponent["metallic"].as<float>();
							*mc.GetMaterial().getFloat("roughness") = materialComponent["roughness"].as<float>();
							*mc.GetMaterial().getFloat("ao") = materialComponent["ao"].as<float>();

							if (hasAlbedo)
							{
								mc.addTexture("albedo", materialComponent["albedoMap"].as<std::string>());
							}

							if (hasNormal)
							{
								mc.addTexture("normal", materialComponent["normalMap"].as<std::string>());
							}

							if (hasMetallic)
							{
								mc.addTexture("metallic", materialComponent["metallicMap"].as<std::string>());
							}

							if (hasRoughness)
							{
								mc.addTexture("roughness", materialComponent["roughnessMap"].as<std::string>());
							}

							if (hasAO)
							{
								mc.addTexture("ao", materialComponent["aoMap"].as<std::string>());
							}
						}

						auto lightComponent = component["LightComponent"];
						if (lightComponent)
						{
							if (lightComponent["lightType"].as<std::string>() == "Directional")
							{
								auto& lc = deserializedEntity->AddComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL);
								lc.dir_color = lightComponent["color"].as<glm::vec3>();
							}

							if (lightComponent["lightType"].as<std::string>() == "Point")
							{
								auto& lc = deserializedEntity->AddComponent<LightComponent>(LightComponent::LightType::POINT);
								lc.point_color = lightComponent["color"].as<glm::vec3>();
							}
						}

						auto cameraComponent = component["CameraComponent"];
						if (cameraComponent)
						{
							auto& cc = deserializedEntity->AddComponent<CameraComponent>();
							cc.Init();
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