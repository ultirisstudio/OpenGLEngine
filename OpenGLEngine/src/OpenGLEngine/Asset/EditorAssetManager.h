#pragma once

#include "AssetManager.h"

namespace OpenGLEngine
{
	class EditorAssetManager : public AssetManager
	{
	public:
		EditorAssetManager();
		~EditorAssetManager();

		virtual void LoadAsset(std::string name) override;
		virtual void UnloadAsset(std::string name) override;

		bool IsAssetLoaded(std::string name) const;

		virtual std::shared_ptr<Asset> GetAsset(std::string name) const override;
	private:
		std::unordered_map<std::string, std::shared_ptr<Asset>> m_LoadedAssets;
	};
}