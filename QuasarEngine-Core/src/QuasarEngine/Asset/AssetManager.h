#pragma once

#include <memory>
#include <string>

#include "Asset.h"

#include <QuasarEngine/Renderer/Renderer.h>

namespace QuasarEngine
{
	class AssetManager
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<Asset>> m_LoadedAssets;
	public:
		void LoadAsset(std::string name);
		void UnloadAsset(std::string name);

		std::shared_ptr<Asset> GetAsset(std::string name);

		bool IsAssetLoaded(std::string name) const;

		template<typename T>
		static std::shared_ptr<T> GetAsset(std::string name)
		{
			return std::dynamic_pointer_cast<T>(Renderer::m_SceneData.m_AssetManager->GetAsset(name));
		}
	};
}