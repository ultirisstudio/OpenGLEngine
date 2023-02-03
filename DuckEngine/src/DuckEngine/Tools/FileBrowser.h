#pragma once

#include <string>

namespace DuckEngine
{
	class FileBrowser
	{
	public:
		struct FileInfo
		{
			std::string m_SelectedFile;
			std::string m_FilePath;
			std::string m_FileExtension;
			std::string m_FileName;

			FileInfo() : m_SelectedFile(""), m_FilePath(""), m_FileExtension(""), m_FileName("") {}
		};

		FileBrowser();
		bool OpenFile();
		FileInfo& GetInfos() { return m_FileInfos; }
	private:
		FileInfo m_FileInfos;
	};
}