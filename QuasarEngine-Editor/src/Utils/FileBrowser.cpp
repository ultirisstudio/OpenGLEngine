#include "qepch.h"
#include <iostream>
#include <fstream>

#include "FileBrowser.h"

#include <tinyfiledialogs/tinyfiledialogs.h>

namespace QuasarEngine
{
	FileBrowser::FileBrowser()
	{
        
	}

	bool FileBrowser::OpenFile()
	{
        Reset();

        char const* result = tinyfd_openFileDialog(
            "Open File",
            "",
            0,
            NULL,
            NULL,
            0
        );

        if (!result)
        {
            std::cout << "No file selected" << std::endl;
            return false;
        }

        std::cout << result << std::endl;
        m_FileInfos.m_FilePath = result;

        const size_t slash = m_FileInfos.m_FilePath.find_last_of("/\\");
        m_FileInfos.m_SelectedFile = m_FileInfos.m_FilePath.substr(slash + 1);

        m_FileInfos.m_FileExtension = m_FileInfos.m_SelectedFile.substr(m_FileInfos.m_SelectedFile.find_last_of(".") + 1);

        size_t lastindex = m_FileInfos.m_SelectedFile.find_last_of(".");
        m_FileInfos.m_FileName = m_FileInfos.m_SelectedFile.substr(0, lastindex);

		return true;
	}

    bool FileBrowser::SaveFile()
    {
        Reset();

        char const* lFilterPatterns[1] = { "*.scene" };

        char const* result = tinyfd_saveFileDialog(
            "Save File",
            "",
            1,
            lFilterPatterns,
            "Scene files"
        );

        if (!result)
        {
            std::cout << "No path selected" << std::endl;
            return false;
        }

        std::cout << result << std::endl;
        m_FileInfos.m_FilePath = result;

        const size_t slash = m_FileInfos.m_FilePath.find_last_of("/\\");
        m_FileInfos.m_SelectedFile = m_FileInfos.m_FilePath.substr(slash + 1);

        m_FileInfos.m_FileExtension = m_FileInfos.m_SelectedFile.substr(m_FileInfos.m_SelectedFile.find_last_of(".") + 1);

        size_t lastindex = m_FileInfos.m_SelectedFile.find_last_of(".");
        m_FileInfos.m_FileName = m_FileInfos.m_SelectedFile.substr(0, lastindex);

        return true;
    }

    char* FileBrowser::OpenFolder()
    {
        return tinyfd_selectFolderDialog("Open", "");
    }

    void FileBrowser::Reset()
    {
        m_FileInfos.m_SelectedFile = "";
        m_FileInfos.m_FilePath = "";
        m_FileInfos.m_FileExtension = "";
        m_FileInfos.m_FileName = "";
    }
}