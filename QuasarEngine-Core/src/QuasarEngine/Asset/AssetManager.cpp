#include "qepch.h"
#include "AssetManager.h"

#include <unordered_map>
#include <filesystem>
#include <fstream>

#include <QuasarEngine/Asset/AssetHeader.h>
#include <QuasarEngine/Resources/Texture.h>

namespace QuasarEngine
{
	namespace AssetManagerUtils
	{
		static AssetType getAssetTypeFromString(const char* type)
		{
			if (strcmp(type, "Texture") == 0) return AssetType::TEXTURE;
			if (strcmp(type, "Mesh") == 0) return AssetType::MESH;
			if (strcmp(type, "Model") == 0) return AssetType::MODEL;
			if (strcmp(type, "QAsset") == 0) return AssetType::QASSET;
			return AssetType::NONE;
		}
	}

	AssetManager::AssetManager()
	{
		m_AssetRegistry = std::make_unique<AssetRegistry>();

		m_ValidExtention = {
			".obj",
			".fbx",
			".glb",
			".gltf",
			".png",
			".jpg",
			".jpeg",
			".qasset"
		};

		m_ExtentionAssetTypes[m_ValidExtention.at(0)] = AssetType::MESH;
		m_ExtentionAssetTypes[m_ValidExtention.at(1)] = AssetType::MESH;
		m_ExtentionAssetTypes[m_ValidExtention.at(2)] = AssetType::MESH;
		m_ExtentionAssetTypes[m_ValidExtention.at(3)] = AssetType::MESH;
		m_ExtentionAssetTypes[m_ValidExtention.at(4)] = AssetType::TEXTURE;
		m_ExtentionAssetTypes[m_ValidExtention.at(5)] = AssetType::TEXTURE;
		m_ExtentionAssetTypes[m_ValidExtention.at(6)] = AssetType::TEXTURE;
		m_ExtentionAssetTypes[m_ValidExtention.at(7)] = AssetType::QASSET;
	}

	bool AssetManager::isAssetRegistered(std::string id)
	{
		return m_AssetRegistry->isAssetRegistred(id);
	}

	void AssetManager::registerAsset(std::string id, AssetType type)
	{
		m_AssetRegistry->registerAsset(id, type);
	}

	AssetType AssetManager::getAssetTypes(std::string id)
	{
		if (m_AssetRegistry->isAssetRegistred(id))
		{
			return m_AssetRegistry->getAssetType(id);
		}
		else
		{
			std::string extention = std::filesystem::path(id).extension().string();
			if (std::find(m_ValidExtention.begin(), m_ValidExtention.end(), extention) != m_ValidExtention.end())
			{
				if (strcmp(extention.c_str(), ".qasset") == 0)
				{
					std::ifstream file(id, std::ios::binary);

					AssetHeader assetHeader;
					file.read(reinterpret_cast<char*>(&assetHeader), sizeof(assetHeader));

					file.close();

					std::string type = assetHeader.assetType;

					return AssetManagerUtils::getAssetTypeFromString(type.c_str());
				}
				else
				{
					return m_ExtentionAssetTypes.at(extention);
				}
			}
		}

		return AssetType();
	}

	void AssetManager::loadAsset(std::string id)
	{
		if (!m_AssetRegistry->isAssetRegistred(id))
		{
			AssetType type = getAssetTypes(id);
			m_AssetRegistry->registerAsset(id, type);
		}

		AssetType type = m_AssetRegistry->getAssetType(id);

		switch (type)
		{
		case AssetType::TEXTURE:
			TextureSpecification spec;
			m_LoadedAssets[id] = Texture::CreateTexture(id, spec);
			break;
		}
	}

	void AssetManager::loadAsset(std::string id, std::shared_ptr<Asset> asset)
	{
		if (!m_AssetRegistry->isAssetRegistred(id))
		{
			m_AssetRegistry->registerAsset(id, asset->GetType());
		}

		if (!isAssetLoaded(id))
			m_LoadedAssets[id] = asset;
	}

	void AssetManager::unloadAsset(std::string id)
	{
		m_LoadedAssets.erase(id);
		m_AssetRegistry->unregisterAsset(id);
	}

	bool AssetManager::isAssetLoaded(std::string id) const
	{
		return (m_LoadedAssets.find(id) != m_LoadedAssets.end());
	}

	std::shared_ptr<Asset> AssetManager::getAsset(std::string id)
	{
		if (!isAssetLoaded(id))
			return nullptr;

		return m_LoadedAssets.at(id);
	}
}