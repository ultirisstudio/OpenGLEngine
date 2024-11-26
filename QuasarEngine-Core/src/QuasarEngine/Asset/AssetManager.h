#pragma once

#include <memory>
#include <string>

#include "Asset.h"
#include "AssetRegistry.h"

#include <QuasarEngine/Resources/Texture.h>
#include <QuasarEngine/Resources/Model.h>

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

		AssetType getTypeFromExtention(const std::string& str);

		static AssetType getAssetTypeFromString(const char* type);

		static std::string getStringFromAssetType(AssetType type);
	};
}