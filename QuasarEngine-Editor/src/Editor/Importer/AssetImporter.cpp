#include "AssetImporter.h"

#include "TextureImporter.h"

#include <QuasarEngine/Core/Log.h>

namespace QuasarEngine
{
	AssetImporter::AssetData AssetImporter::m_AssetData = AssetImporter::AssetData();

	void AssetImporter::Init()
	{
		m_AssetData.m_ValidExtention = {
			".obj",
			".fbx",
			".glb",
			".gltf",
			".png",
			".jpg",
			".jpeg"
		};

		m_AssetData.m_ImportFunction.insert({ m_AssetData.m_ValidExtention.at(0), &TextureImporter::Test });
	}

	void AssetImporter::ImportAsset(std::filesystem::path path)
	{
		//Log::log(Log::INFO, path.extension().string());
		//Q_ASSERT(strcmp(path.extension().string().c_str(), ".obj") == 0, "Not a obj file !");
		if (std::find(m_AssetData.m_ValidExtention.begin(), m_AssetData.m_ValidExtention.end(), path.extension().string()) != m_AssetData.m_ValidExtention.end())
		{
			Log::log(Log::INFO, "extention valid");
			std::unordered_map<std::string, std::function<std::vector<char>(std::filesystem::path)>>::iterator it = m_AssetData.m_ImportFunction.find(path.extension().string());
			if (it != m_AssetData.m_ImportFunction.end())
			{
				Log::log(Log::INFO, "call function");
				std::vector<char> data = it->second(path);
			}
		}
	}
}