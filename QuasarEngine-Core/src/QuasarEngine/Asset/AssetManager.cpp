#include "qepch.h"
#include "AssetManager.h"

#include <unordered_map>

namespace QuasarEngine
{
	void AssetManager::LoadAsset(std::string name)
	{

	}

	void AssetManager::UnloadAsset(std::string name)
	{

	}

	bool AssetManager::IsAssetLoaded(std::string name) const
	{
		return (m_LoadedAssets.find(name) != m_LoadedAssets.end());
	}

	std::shared_ptr<Asset> AssetManager::GetAsset(std::string name)
	{
		if (!IsAssetLoaded(name))
			return nullptr;

		return m_LoadedAssets.at(name);
	}
}