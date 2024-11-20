#include "SceneSerializer.h"
#include "Tools/Utils.h"

#include <yaml-cpp/yaml.h>

#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/HierarchyComponent.h>
#include <QuasarEngine/Entity/Components/TransformComponent.h>
#include <QuasarEngine/Entity/Components/MaterialComponent.h>
#include <QuasarEngine/Entity/Components/MeshComponent.h>
#include <QuasarEngine/Entity/Components/CameraComponent.h>
#include <QuasarEngine/Entity/Components/LightComponent.h>
#include <QuasarEngine/Entity/Components/MeshRendererComponent.h>
#include <QuasarEngine/Entity/Components/Physics/RigidBodyComponent.h>
#include <QuasarEngine/Entity/Components/Physics/MeshColliderComponent.h>
#include <QuasarEngine/Entity/Components/Physics/BoxColliderComponent.h>
#include <QuasarEngine/Entity/Components/Physics/CapsuleColliderComponent.h>
#include <QuasarEngine/Entity/Components/Gameplay/CharacterControllerComponent.h>

#include <QuasarEngine/Core/Input.h>
#include <QuasarEngine/Core/KeyCodes.h>

#include "Importer/TextureConfigImporter.h"

#include <fstream>

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

namespace QuasarEngine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity, const std::string& assetPath)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetName();
		out << YAML::Key << "ID" << YAML::Value << entity.GetUUID();
		uint64_t parentID = ((entity.GetComponent<HierarchyComponent>().m_Parent != UUID::Null()) ? entity.GetComponent<HierarchyComponent>().m_Parent : UUID::Null());
		out << YAML::Key << "ParentID" << YAML::Value << parentID;

		out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "TransformComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<MaterialComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "MaterialComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mc = entity.GetComponent<MaterialComponent>();

			bool hasAlbedo = mc.GetMaterial().HasTexture(TextureType::Albedo);
			bool hasNormal = mc.GetMaterial().HasTexture(TextureType::Normal);
			bool hasMetallic = mc.GetMaterial().HasTexture(TextureType::Metallic);
			bool hasRoughness = mc.GetMaterial().HasTexture(TextureType::Roughness);
			bool hasAO = mc.GetMaterial().HasTexture(TextureType::AO);

			out << YAML::Key << "albedo" << YAML::Value << mc.GetMaterial().GetSpecification().Albedo;
			out << YAML::Key << "metallic" << YAML::Value << mc.GetMaterial().GetSpecification().Metallic;
			out << YAML::Key << "roughness" << YAML::Value << mc.GetMaterial().GetSpecification().Roughness;
			out << YAML::Key << "ao" << YAML::Value << mc.GetMaterial().GetSpecification().AO;

			if (hasAlbedo)
			{
				std::optional<std::string> relativePath = Utils::getRelativePath(mc.GetMaterial().GetSpecification().AlbedoTexture.value(), assetPath);
				out << YAML::Key << "albedoMap" << YAML::Value << (relativePath.has_value() ? relativePath.value() : "");
			}

			if (hasNormal)
			{
				std::optional<std::string> relativePath = Utils::getRelativePath(mc.GetMaterial().GetSpecification().NormalTexture.value(), assetPath);
				out << YAML::Key << "normalMap" << YAML::Value << (relativePath.has_value() ? relativePath.value() : "");
			}

			if (hasMetallic)
			{
				std::optional<std::string> relativePath = Utils::getRelativePath(mc.GetMaterial().GetSpecification().MetallicTexture.value(), assetPath);
				out << YAML::Key << "metallicMap" << YAML::Value << (relativePath.has_value() ? relativePath.value() : "");
			}

			if (hasRoughness)
			{
				std::optional<std::string> relativePath = Utils::getRelativePath(mc.GetMaterial().GetSpecification().RoughnessTexture.value(), assetPath);
				out << YAML::Key << "roughnessMap" << YAML::Value << (relativePath.has_value() ? relativePath.value() : "");
			}

			if (hasAO)
			{
				std::optional<std::string> relativePath = Utils::getRelativePath(mc.GetMaterial().GetSpecification().AOTexture.value(), assetPath);
				out << YAML::Key << "aoMap" << YAML::Value << (relativePath.has_value() ? relativePath.value() : "");
			}

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshRendererComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "MeshRendererComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mrc = entity.GetComponent<MeshRendererComponent>();

			out << YAML::Key << "Rendered" << YAML::Value << mrc.m_Rendered;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "MeshComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mc = entity.GetComponent<MeshComponent>();

			std::optional<std::string> relativePath = Utils::getRelativePath(mc.GetModelPath(), assetPath);
			out << YAML::Key << "Path" << YAML::Value << (relativePath.has_value() ? relativePath.value() : "");
			out << YAML::Key << "Name" << YAML::Value << mc.GetName();

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<LightComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "LightComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& lc = entity.GetComponent<LightComponent>();

			if (lc.lightType == LightComponent::LightType::DIRECTIONAL)
			{
				out << YAML::Key << "lightType" << YAML::Value << "Directional";

				out << YAML::Key << "color" << YAML::Value << lc.dir_color;
				out << YAML::Key << "power" << YAML::Value << lc.dir_power;
			}
			else if (lc.lightType == LightComponent::LightType::POINT)
			{
				out << YAML::Key << "lightType" << YAML::Value << "Point";

				out << YAML::Key << "color" << YAML::Value << lc.point_color;
				out << YAML::Key << "attenuation" << YAML::Value << lc.point_attenuation;
				out << YAML::Key << "power" << YAML::Value << lc.point_power;
			}

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "CameraComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& cc = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "fov" << YAML::Value << cc.GetCamera().GetFov();

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBodyComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "RigidBodyComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& rbc = entity.GetComponent<RigidBodyComponent>();
			out << YAML::Key << "enableGravity" << YAML::Value << rbc.enableGravity;
			out << YAML::Key << "bodyType" << YAML::Value << rbc.bodyTypeString;
			out << YAML::Key << "linearAxisFactor_X" << YAML::Value << rbc.m_LinearAxisFactorX;
			out << YAML::Key << "linearAxisFactor_Y" << YAML::Value << rbc.m_LinearAxisFactorY;
			out << YAML::Key << "linearAxisFactor_Z" << YAML::Value << rbc.m_LinearAxisFactorZ;
			out << YAML::Key << "angularAxisFactor_X" << YAML::Value << rbc.m_AngularAxisFactorX;
			out << YAML::Key << "angularAxisFactor_Y" << YAML::Value << rbc.m_AngularAxisFactorY;
			out << YAML::Key << "angularAxisFactor_Z" << YAML::Value << rbc.m_AngularAxisFactorZ;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent< CharacterControllerComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "CharacterControllerComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& ccc = entity.GetComponent<CharacterControllerComponent>();

			out << YAML::Key << "linearAxisFactor_X" << YAML::Value << ccc.m_LinearAxisFactorX;
			out << YAML::Key << "linearAxisFactor_Y" << YAML::Value << ccc.m_LinearAxisFactorY;
			out << YAML::Key << "linearAxisFactor_Z" << YAML::Value << ccc.m_LinearAxisFactorZ;

			out << YAML::Key << "angularAxisFactor_X" << YAML::Value << ccc.m_AngularAxisFactorX;
			out << YAML::Key << "angularAxisFactor_Y" << YAML::Value << ccc.m_AngularAxisFactorY;
			out << YAML::Key << "angularAxisFactor_Z" << YAML::Value << ccc.m_AngularAxisFactorZ;

			out << YAML::Key << "mass" << YAML::Value << ccc.mass;
			out << YAML::Key << "friction" << YAML::Value << ccc.friction;
			out << YAML::Key << "bounciness" << YAML::Value << ccc.bounciness;

			out << YAML::Key << "radius" << YAML::Value << ccc.m_Radius;
			out << YAML::Key << "height" << YAML::Value << ccc.m_Height;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshColliderComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "MeshColliderComponent";
			out << YAML::Value << YAML::BeginMap;

			auto& mcc = entity.GetComponent<MeshColliderComponent>();
			out << YAML::Key << "IsConvex" << YAML::Value << mcc.m_IsConvex;

			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxColliderComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "BoxColliderComponent";
			out << YAML::Value << YAML::BeginMap;
			auto& bcc = entity.GetComponent<BoxColliderComponent>();
			out << YAML::Key << "mass" << YAML::Value << bcc.mass;
			out << YAML::Key << "friction" << YAML::Value << bcc.friction;
			out << YAML::Key << "bounciness" << YAML::Value << bcc.bounciness;
			out << YAML::Key << "useEntityScale" << YAML::Value << bcc.m_UseEntityScale;
			out << YAML::Key << "size" << YAML::Value << bcc.m_Size;
			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CapsuleColliderComponent>())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "CapsuleColliderComponent";
			out << YAML::Value << YAML::BeginMap;
			auto& ccc = entity.GetComponent<CapsuleColliderComponent>();
			out << YAML::Key << "mass" << YAML::Value << ccc.mass;
			out << YAML::Key << "friction" << YAML::Value << ccc.friction;
			out << YAML::Key << "bounciness" << YAML::Value << ccc.bounciness;
			out << YAML::Key << "radius" << YAML::Value << ccc.m_Radius;
			out << YAML::Key << "height" << YAML::Value << ccc.m_Height;
			out << YAML::EndMap;
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::vector<UUID> childrens = entity.GetComponent<HierarchyComponent>().m_Childrens;

		for (auto& child : childrens)
		{
			Entity childEntity = Renderer::m_SceneData.m_Scene->GetEntityByUUID(child);
			SerializeEntity(out, childEntity, assetPath);
		}
	}

	SceneSerializer::SceneSerializer(SceneObject& sceneObject, std::filesystem::path assetPath) : m_SceneObject(&sceneObject), m_AssetPath(assetPath)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_SceneObject->GetName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto e : m_SceneObject->GetScene().GetAllEntitiesWith<IDComponent>())
		{
			Entity entity(e, m_SceneObject->GetScene().GetRegistry());
			if (entity.GetComponent<HierarchyComponent>().m_Parent == UUID::Null())
				SerializeEntity(out, entity, m_AssetPath.string());
		};

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

		m_SceneObject->SetPath(filepath);
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
	}

	static void DeserializeEntity(YAML::Node entity, Scene* scene, const std::string& assetPath)
	{
		std::string name = entity["Entity"].as<std::string>();
		UUID uuid = entity["ID"].as<uint64_t>();
		UUID parent_uuid = entity["ParentID"].as<uint64_t>();

		Entity deserializedEntity = scene->CreateEntityWithUUID(uuid, name);
		Entity deserializedEntityParent = scene->GetEntityByUUID(parent_uuid);

		if (deserializedEntityParent)
		{
			deserializedEntity.GetComponent<HierarchyComponent>().m_Parent = deserializedEntityParent.GetUUID();
			deserializedEntityParent.GetComponent<HierarchyComponent>().AddChild(deserializedEntityParent.GetUUID(), deserializedEntity.GetUUID());
		}

		auto components = entity["Components"];
		if (components)
		{
			for (auto component : components)
			{
				auto transformComponent = component["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.AddComponent<TransformComponent>();
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto meshRendererComponent = component["MeshRendererComponent"];
				if (meshRendererComponent)
				{
					auto& mrc = deserializedEntity.AddComponent<MeshRendererComponent>();
					auto rendered = meshRendererComponent["Rendered"];
					if (rendered)
						mrc.m_Rendered = rendered.as<bool>();
				}

				auto meshComponent = component["MeshComponent"];
				if (meshComponent)
				{
					std::string path = assetPath + "\\" + meshComponent["Path"].as<std::string>();
					std::string name = meshComponent["Name"].as<std::string>();

					Mesh* mesh;

					if (Renderer::m_SceneData.m_ResourceManager->GetModel(path))
					{
						mesh = Renderer::m_SceneData.m_ResourceManager->GetModel(path)->GetMesh(name);
					}
					else
					{
						mesh = Renderer::m_SceneData.m_ResourceManager->CreateModel(path)->GetMesh(name);
					}

					auto& mc = deserializedEntity.AddComponent<MeshComponent>(name, mesh, path);
				}

				// TODO : Remove this
				if (!meshRendererComponent && meshComponent)
				{
					auto& mrc = deserializedEntity.AddComponent<MeshRendererComponent>();
					mrc.m_Rendered = true;
				}

				auto materialComponent = component["MaterialComponent"];
				if (materialComponent)
				{
					MaterialSpecification spec;

					spec.Albedo = materialComponent["albedo"].as<glm::vec3>();
					spec.Metallic = materialComponent["metallic"].as<float>();
					spec.Roughness = materialComponent["roughness"].as<float>();
					spec.AO = materialComponent["ao"].as<float>();

					auto hasAlbedo = materialComponent["albedoMap"];
					if (hasAlbedo)
					{
						std::string albedoPath = assetPath + "\\" + hasAlbedo.as<std::string>();
						spec.AlbedoTexture = albedoPath;
						if (!Renderer::m_SceneData.m_AssetManager->isAssetLoaded(albedoPath))
						{
							Renderer::m_SceneData.m_AssetManager->loadAsset(albedoPath);
						}
					}

					auto hasNormal = materialComponent["normalMap"];
					if (hasNormal)
					{
						std::string normalPath = assetPath + "\\" + hasNormal.as<std::string>();
						spec.NormalTexture = normalPath;
						if (!Renderer::m_SceneData.m_AssetManager->isAssetLoaded(normalPath))
						{
							Renderer::m_SceneData.m_AssetManager->loadAsset(normalPath);
						}
					}

					auto hasMetallic = materialComponent["metallicMap"];
					if (hasMetallic)
					{
						std::string metallicPath = assetPath + "\\" + hasMetallic.as<std::string>();
						spec.MetallicTexture = metallicPath;
						if (!Renderer::m_SceneData.m_AssetManager->isAssetLoaded(metallicPath))
						{
							Renderer::m_SceneData.m_AssetManager->loadAsset(metallicPath);
						}
					}

					auto hasRoughness = materialComponent["roughnessMap"];
					if (hasRoughness)
					{
						std::string roughnessPath = assetPath + "\\" + hasRoughness.as<std::string>();
						spec.RoughnessTexture = roughnessPath;
						if (!Renderer::m_SceneData.m_AssetManager->isAssetLoaded(roughnessPath))
						{
							Renderer::m_SceneData.m_AssetManager->loadAsset(roughnessPath);
						}
					}

					auto hasAO = materialComponent["aoMap"];
					if (hasAO)
					{
						std::string aoPath = assetPath + "\\" + hasAO.as<std::string>();
						spec.AOTexture = aoPath;
						if (!Renderer::m_SceneData.m_AssetManager->isAssetLoaded(aoPath))
						{
							Renderer::m_SceneData.m_AssetManager->loadAsset(aoPath);
						}
					}

					auto& mc = deserializedEntity.AddComponent<MaterialComponent>(spec);
				}

				auto lightComponent = component["LightComponent"];
				if (lightComponent)
				{
					if (lightComponent["lightType"].as<std::string>() == "Directional")
					{
						auto& lc = deserializedEntity.AddComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL);
						lc.dir_color = lightComponent["color"].as<glm::vec3>();

						if (lightComponent["power"])
							lc.dir_power = lightComponent["power"].as<float>();
					}

					if (lightComponent["lightType"].as<std::string>() == "Point")
					{
						auto& lc = deserializedEntity.AddComponent<LightComponent>(LightComponent::LightType::POINT);
						lc.point_color = lightComponent["color"].as<glm::vec3>();

						if (lightComponent["attenuation"])
							lc.point_attenuation = lightComponent["attenuation"].as<float>();

						if (lightComponent["power"])
							lc.point_power = lightComponent["power"].as<float>();
					}
				}

				auto cameraComponent = component["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					cc.GetCamera().Init(&deserializedEntity.GetComponent<TransformComponent>());
					cc.GetCamera().SetFov(cameraComponent["fov"].as<float>());
				}

				auto rigidBodyComponent = component["RigidBodyComponent"];
				if (rigidBodyComponent)
				{
					auto& rbc = deserializedEntity.AddComponent<RigidBodyComponent>();
					rbc.Init();

					rbc.enableGravity = rigidBodyComponent["enableGravity"].as<bool>();
					rbc.bodyTypeString = rigidBodyComponent["bodyType"].as<std::string>();

					if (rigidBodyComponent["linearAxisFactor_X"])
						rbc.m_LinearAxisFactorX = rigidBodyComponent["linearAxisFactor_X"].as<bool>();

					if (rigidBodyComponent["linearAxisFactor_Y"])
						rbc.m_LinearAxisFactorY = rigidBodyComponent["linearAxisFactor_Y"].as<bool>();

					if (rigidBodyComponent["linearAxisFactor_Z"])
						rbc.m_LinearAxisFactorZ = rigidBodyComponent["linearAxisFactor_Z"].as<bool>();

					if (rigidBodyComponent["angularAxisFactor_X"])
						rbc.m_AngularAxisFactorX = rigidBodyComponent["angularAxisFactor_X"].as<bool>();

					if (rigidBodyComponent["angularAxisFactor_Y"])
						rbc.m_AngularAxisFactorY = rigidBodyComponent["angularAxisFactor_Y"].as<bool>();

					if (rigidBodyComponent["angularAxisFactor_Z"])
						rbc.m_AngularAxisFactorZ = rigidBodyComponent["angularAxisFactor_Z"].as<bool>();

					rbc.UpdateEnableGravity();
					rbc.UpdateBodyType();
					rbc.UpdateLinearAxisFactor();
					rbc.UpdateAngularAxisFactor();
				}

				auto characterControllerComponent = component["CharacterControllerComponent"];
				if (characterControllerComponent)
				{
					auto& ccc = deserializedEntity.AddComponent<CharacterControllerComponent>();
					ccc.Init();

					if (characterControllerComponent["linearAxisFactor_X"])
						ccc.m_LinearAxisFactorX = characterControllerComponent["linearAxisFactor_X"].as<bool>();

					if (characterControllerComponent["linearAxisFactor_Y"])
						ccc.m_LinearAxisFactorY = characterControllerComponent["linearAxisFactor_Y"].as<bool>();

					if (characterControllerComponent["linearAxisFactor_Z"])
						ccc.m_LinearAxisFactorZ = characterControllerComponent["linearAxisFactor_Z"].as<bool>();

					if (characterControllerComponent["angularAxisFactor_X"])
						ccc.m_AngularAxisFactorX = characterControllerComponent["angularAxisFactor_X"].as<bool>();

					if (characterControllerComponent["angularAxisFactor_Y"])
						ccc.m_AngularAxisFactorY = characterControllerComponent["angularAxisFactor_Y"].as<bool>();

					if (characterControllerComponent["angularAxisFactor_Z"])
						ccc.m_AngularAxisFactorZ = characterControllerComponent["angularAxisFactor_Z"].as<bool>();

					if (characterControllerComponent["mass"])
						ccc.mass = characterControllerComponent["mass"].as<float>();

					if (characterControllerComponent["friction"])
						ccc.friction = characterControllerComponent["friction"].as<float>();

					if (characterControllerComponent["bounciness"])
						ccc.bounciness = characterControllerComponent["bounciness"].as<float>();

					if (characterControllerComponent["radius"])
						ccc.m_Radius = characterControllerComponent["radius"].as<float>();

					if (characterControllerComponent["height"])
						ccc.m_Height = characterControllerComponent["height"].as<float>();

					ccc.UpdateColliderSize();
					ccc.UpdateColliderMaterial();
					ccc.UpdateLinearAxisFactor();
					ccc.UpdateAngularAxisFactor();
				}

				auto meshColliderComponent = component["MeshColliderComponent"];
				if (meshColliderComponent)
				{
					auto& mcc = deserializedEntity.AddComponent<MeshColliderComponent>();
					mcc.m_IsConvex = meshColliderComponent["IsConvex"].as<bool>();
					mcc.Generate();
				}

				auto boxColliderComponent = component["BoxColliderComponent"];
				if (boxColliderComponent)
				{
					auto& bcc = deserializedEntity.AddComponent<BoxColliderComponent>();
					bcc.Init();

					if (boxColliderComponent["mass"])
						bcc.mass = boxColliderComponent["mass"].as<float>();

					if (boxColliderComponent["friction"])
						bcc.friction = boxColliderComponent["friction"].as<float>();

					if (boxColliderComponent["bounciness"])
						bcc.bounciness = boxColliderComponent["bounciness"].as<float>();

					if (boxColliderComponent["useEntityScale"])
						bcc.m_UseEntityScale = boxColliderComponent["useEntityScale"].as<bool>();

					if (boxColliderComponent["size"])
						bcc.m_Size = boxColliderComponent["size"].as<glm::vec3>();

					bcc.UpdateColliderMaterial();
					bcc.UpdateColliderSize();
				}

				auto capsuleColliderComponent = component["CapsuleColliderComponent"];
				if (capsuleColliderComponent)
				{
					auto& ccc = deserializedEntity.AddComponent<CapsuleColliderComponent>();
					ccc.Init();

					if (capsuleColliderComponent["mass"])
						ccc.mass = capsuleColliderComponent["mass"].as<float>();

					if (capsuleColliderComponent["friction"])
						ccc.friction = capsuleColliderComponent["friction"].as<float>();

					if (capsuleColliderComponent["bounciness"])
						ccc.bounciness = capsuleColliderComponent["bounciness"].as<float>();

					if (capsuleColliderComponent["radius"])
						ccc.m_Radius = capsuleColliderComponent["radius"].as<float>();

					if (capsuleColliderComponent["height"])
						ccc.m_Height = capsuleColliderComponent["height"].as<float>();

					ccc.UpdateColliderMaterial();
					ccc.UpdateColliderSize();
				}
			}
		}
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
		m_SceneObject->SetName(sceneName);
		m_SceneObject->SetPath(filepath);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				//if (entity["ParentID"].as<uint64_t>() != 0)
					DeserializeEntity(entity, &m_SceneObject->GetScene(), m_AssetPath.string());
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
		{
			return false;
		}

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				//if (entity["ParentID"].as<uint64_t>() != 0)
					DeserializeEntity(entity, &m_SceneObject->GetScene(), m_AssetPath.string());
			}
		}

		return true;
	}
}