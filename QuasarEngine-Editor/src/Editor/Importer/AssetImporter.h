#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>
#include <functional>

#include <QuasarEngine/Asset/Asset.h>

namespace QuasarEngine
{
	class Texture;

	class AssetImporter
	{
	private:
		typedef std::unordered_map<std::string, std::function<std::shared_ptr<Texture>(const std::string&)>> ImportFunction;
		typedef std::unordered_map<std::string, std::function<void(const std::string&, const std::string&)>> ExportFunction;

		ImportFunction m_ImportFunctions;
		ExportFunction m_ExportFunctions;

		std::vector<std::string> m_ValidExtention;

		std::string m_ProjectPath;
	public:
		AssetImporter(const std::string& projectPath);

		void ImportAsset();
		void ImportAsset(std::filesystem::path path);

		AssetType getAssetType(std::filesystem::path path);
	};
}