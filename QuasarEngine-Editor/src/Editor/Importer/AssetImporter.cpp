#include "AssetImporter.h"

#include "TextureImporter.h"

#include "Tools/Utils.h"

#include <QuasarEngine/Core/Log.h>
#include <QuasarEngine/Resources/Texture.h>
#include <QuasarEngine/Renderer/Renderer.h>

namespace QuasarEngine
{
	AssetImporter::AssetImporter(const std::string& projectPath) : m_ProjectPath(projectPath)
	{
		m_ValidExtention = {
			//".obj",
			//".fbx",
			//".glb",
			//".gltf",
			".png",
			".jpg",
			".jpeg"
		};

		m_ImportFunctions[m_ValidExtention.at(0)] = &TextureImporter::importTexture;
		m_ExportFunctions[m_ValidExtention.at(0)] = &TextureImporter::exportTexture;
	}

	void AssetImporter::ImportAsset()
	{
		std::optional<Utils::FileInfo> infos = Utils::openFile();

		if (infos.has_value())
		{
			if (std::find(m_ValidExtention.begin(), m_ValidExtention.end(), infos.value().fileExtension) != m_ValidExtention.end())
			{
				ExportFunction::iterator it = m_ExportFunctions.find(infos.value().fileExtension);
				if (it != m_ExportFunctions.end())
				{
					std::string out = std::string(m_ProjectPath + "\\Assets\\" + infos.value().fileName + ".qasset");
					it->second(infos.value().filePath, out);
					Log::log(Log::INFO, std::string(infos.value().selectedFile + " imported on " + out));
				}
			}
		}
	}

	void AssetImporter::ImportAsset(std::filesystem::path path)
	{
		if (std::find(m_ValidExtention.begin(), m_ValidExtention.end(), path.extension().string()) != m_ValidExtention.end())
		{
			/*Log::log(Log::INFO, "extention valid");
			std::unordered_map<std::string, std::function<std::vector<char>(std::filesystem::path)>>::iterator it = m_AssetData.m_ImportFunction.find(path.extension().string());
			if (it != m_AssetData.m_ImportFunction.end())
			{
				Log::log(Log::INFO, "call function");
				std::vector<char> data = it->second(path);
			}*/
		}
	}
}