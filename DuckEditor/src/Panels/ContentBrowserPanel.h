#pragma once

#include <DuckEngine/Resources/Texture.h>

#include <filesystem>

namespace DuckEngine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

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
	};
}