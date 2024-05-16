#pragma once

#include <OpenGLEngine/Resources/Texture.h>

#include <filesystem>

#include "TextureViewerPanel.h"

namespace OpenGLEngine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(const std::string& projectPath);

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
		std::shared_ptr<Texture> m_Temp;

		std::shared_ptr<TextureViewerPanel> m_TextureViewerPanel;
	};
}