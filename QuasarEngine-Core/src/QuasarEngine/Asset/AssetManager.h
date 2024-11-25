#pragma once

#include <memory>
#include <string>

#include "Asset.h"
#include "AssetRegistry.h"

namespace QuasarEngine
{
	class AssetManager
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<Asset>> m_LoadedAssets;

		std::unique_ptr<AssetRegistry> m_AssetRegistry;

		std::vector<std::string> m_ValidExtention;
		std::unordered_map<std::string, AssetType> m_ExtentionAssetTypes;
	public:
		AssetManager();

		bool isAssetRegistered(std::string id);
		void registerAsset(std::string id, AssetType type);

		AssetType getAssetType(std::string id);

		void loadAsset(std::string id);
		void loadAsset(std::string id, std::shared_ptr<Asset> asset);
		void unloadAsset(std::string id);

		std::shared_ptr<Asset> getAsset(std::string id);

		bool isAssetLoaded(std::string id) const;

		template<typename T>
		std::shared_ptr<T> getAsset(std::string id)
		{
			return std::dynamic_pointer_cast<T>(getAsset(id));
		}

		static AssetType getAssetTypeFromString(const char* type)
		{
			if (strcmp(type, "Texture") == 0) return AssetType::TEXTURE;
			if (strcmp(type, "Mesh") == 0) return AssetType::MESH;
			if (strcmp(type, "Model") == 0) return AssetType::MODEL;
			if (strcmp(type, "QAsset") == 0) return AssetType::QASSET;
			return AssetType::NONE;
		}

		static std::string getStringFromAssetType(AssetType type)
		{
			switch (type)
			{
			case AssetType::TEXTURE: return "Texture";
			case AssetType::MESH: return "Mesh";
			case AssetType::MODEL: return "Model";
			case AssetType::QASSET: return "QAsset";
			}

			return "None";
		}
	};
}