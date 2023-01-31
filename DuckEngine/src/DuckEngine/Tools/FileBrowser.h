#pragma once

#include <string>

namespace DuckEngine
{
	class FileBrowser
	{
	public:
		struct FileInfo
		{
			std::string sSelectedFile;
			std::string sFilePath;

			FileInfo() : sSelectedFile(""), sFilePath("") {}
		};

		FileBrowser();
		bool OpenFile();
		FileInfo& GetInfos() { return m_FileInfos; }
	private:
		FileInfo m_FileInfos;
	};
}