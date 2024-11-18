#pragma once

#include <filesystem>

#include "TextureViewerPanel.h"

#include <QuasarEngine/Resources/Texture.h>
#include "Editor/Importer/AssetImporter.h"

namespace QuasarEngine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(const std::string& projectPath, AssetImporter* importer);

		void OnImGuiRender();
	private:
		const std::string GetFileExtension(std::filesystem::directory_entry e);

		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		std::shared_ptr<Texture> m_DirectoryIcon;
		std::shared_ptr<Texture> m_FilePNGIcon;
		std::shared_ptr<Texture> m_FileJPGIcon;
		std::shared_ptr<Texture> m_FileOBJIcon;
		std::shared_ptr<Texture> m_FileOtherIcon;

		std::shared_ptr<TextureViewerPanel> m_TextureViewerPanel;

		AssetImporter* m_AssetImporter;
	};
}