#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <functional>

namespace QuasarEngine
{
	class AssetImporter
	{
	public:
		struct AssetData
		{
			std::vector<std::string> m_ValidExtention;
			std::unordered_map<std::string, std::function<std::vector<char>(std::filesystem::path)>> m_ImportFunction;
		};
		static AssetData m_AssetData;

		static void Init();

		static void ImportAsset(std::filesystem::path path);
	};
}