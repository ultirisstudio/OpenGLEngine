#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Asset.h"

#include <QuasarEngine/Renderer/Renderer.h>

namespace QuasarEngine
{
	class AssetManager
	{
	public:
		virtual void LoadAsset(std::string name) = 0;
		virtual void UnloadAsset(std::string name) = 0;

		virtual std::shared_ptr<Asset> GetAsset(std::string name) const = 0;

		template<typename T>
		static std::shared_ptr<T> GetAsset(std::string name)
		{
			return std::dynamic_pointer_cast<T>(Renderer::m_SceneData.m_AssetManager->GetAsset(name));
		}
	};
}