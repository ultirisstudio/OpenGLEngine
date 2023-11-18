#include "depch.h"
#include "FileBrowser.h"
#include <shobjidl.h> 
#include <iostream>
#include <fstream>

namespace OpenGLEngine
{
	FileBrowser::FileBrowser()
	{

	}

	bool FileBrowser::OpenFile()
	{
		HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(f_SysHr))
			return FALSE;

		IFileOpenDialog* f_FileSystem;
		f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
		if (FAILED(f_SysHr)) {
			CoUninitialize();
			return FALSE;
		}

		f_SysHr = f_FileSystem->Show(NULL);
		if (FAILED(f_SysHr)) {
			f_FileSystem->Release();
			CoUninitialize();
			return FALSE;
		}

		IShellItem* f_Files;
		f_SysHr = f_FileSystem->GetResult(&f_Files);
		if (FAILED(f_SysHr)) {
			f_FileSystem->Release();
			CoUninitialize();
			return FALSE;
		}

		PWSTR f_Path;
		f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
		if (FAILED(f_SysHr)) {
			f_Files->Release();
			f_FileSystem->Release();
			CoUninitialize();
			return FALSE;
		}

		std::wstring path(f_Path);
		std::string c(path.begin(), path.end());
		m_FileInfos.m_FilePath = c;

		const size_t slash = m_FileInfos.m_FilePath.find_last_of("/\\");
		m_FileInfos.m_SelectedFile = m_FileInfos.m_FilePath.substr(slash + 1);

		CoTaskMemFree(f_Path);
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();

		m_FileInfos.m_FileExtension = m_FileInfos.m_SelectedFile.substr(m_FileInfos.m_SelectedFile.find_last_of(".") + 1);

		size_t lastindex = m_FileInfos.m_SelectedFile.find_last_of(".");
		m_FileInfos.m_FileName = m_FileInfos.m_SelectedFile.substr(0, lastindex);
		
		return TRUE;
	}

    bool FileBrowser::SaveFile()
    {
        const wchar_t* defaultExtension = L".scene";
        const UINT defaultFileTypeIndex = 1;

        // Initialisation de COM
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr))
        {
            std::cerr << "Erreur lors de l'initialisation de COM." << std::endl;
            return 1;
        }

        // Création de l'instance de IFileSaveDialog
        IFileSaveDialog* pFileSaveDialog;
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileSaveDialog));
        if (FAILED(hr))
        {
            std::cerr << "Impossible de créer l'instance de IFileSaveDialog." << std::endl;
            CoUninitialize();
            return 1;
        }

        // Définition des options de l'interface de dialogue
        DWORD dwFlags;
        hr = pFileSaveDialog->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            // Définir les options supplémentaires si nécessaire
            dwFlags |= FOS_FORCEFILESYSTEM; // Utiliser uniquement des fichiers système
            pFileSaveDialog->SetOptions(dwFlags);
        }

        // Définir l'extension par défaut
        pFileSaveDialog->SetDefaultExtension(defaultExtension);

        // Définir les filtres de types de fichiers
        COMDLG_FILTERSPEC fileTypes[] = {
            { L"Fichiers de sauvegarde", L"*.scene" }
        };
        pFileSaveDialog->SetFileTypes(1, fileTypes);

        // Définir le type de fichier par défaut dans le menu déroulant
        pFileSaveDialog->SetFileTypeIndex(defaultFileTypeIndex);

        // Affichage de la boîte de dialogue de sauvegarde
        hr = pFileSaveDialog->Show(NULL);
        if (SUCCEEDED(hr))
        {
            // Récupération du fichier sélectionné
            IShellItem* pItem;
            hr = pFileSaveDialog->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr))
                {
                    ////////////////////////////////////////////////////////////////////
                    std::wstring path(pszFilePath);
                    std::string c(path.begin(), path.end());
                    m_FileInfos.m_FilePath = c;

                    CoTaskMemFree(pszFilePath);
                }

                pItem->Release();
            }
        }

        // Libération des ressources
        pFileSaveDialog->Release();
        CoUninitialize();

        return true;
    }

    bool FileBrowser::SaveFile(const char* file)
    {
        const wchar_t* defaultExtension = L".scene";
        const UINT defaultFileTypeIndex = 1;
        wchar_t textToSave[100];
        mbstowcs(textToSave, file, strlen(file) + 1);

        // Initialisation de COM
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr))
        {
            std::cerr << "Erreur lors de l'initialisation de COM." << std::endl;
            return 1;
        }

        // Création de l'instance de IFileSaveDialog
        IFileSaveDialog* pFileSaveDialog;
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileSaveDialog));
        if (FAILED(hr))
        {
            std::cerr << "Impossible de créer l'instance de IFileSaveDialog." << std::endl;
            CoUninitialize();
            return 1;
        }

        // Définition des options de l'interface de dialogue
        DWORD dwFlags;
        hr = pFileSaveDialog->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            // Définir les options supplémentaires si nécessaire
            dwFlags |= FOS_FORCEFILESYSTEM; // Utiliser uniquement des fichiers système
            pFileSaveDialog->SetOptions(dwFlags);
        }

        // Définir l'extension par défaut
        pFileSaveDialog->SetDefaultExtension(defaultExtension);

        // Définir les filtres de types de fichiers
        COMDLG_FILTERSPEC fileTypes[] = {
            { L"Fichiers de sauvegarde", L"*.scene" }
        };
        pFileSaveDialog->SetFileTypes(1, fileTypes);

        // Définir le type de fichier par défaut dans le menu déroulant
        pFileSaveDialog->SetFileTypeIndex(defaultFileTypeIndex);

        // Affichage de la boîte de dialogue de sauvegarde
        hr = pFileSaveDialog->Show(NULL);
        if (SUCCEEDED(hr))
        {
            // Récupération du fichier sélectionné
            IShellItem* pItem;
            hr = pFileSaveDialog->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr))
                {
                    ////////////////////////////////////////////////////////////////////
                    std::wstring path(pszFilePath);
                    std::string c(path.begin(), path.end());
                    m_FileInfos.m_FilePath = c;

                    // Enregistrement du texte dans le fichier
                    std::wofstream fileStream(pszFilePath);
                    if (fileStream.is_open())
                    {
                        fileStream << textToSave;
                        fileStream.close();
                        std::wcout << "Fichier enregistré avec succès : " << pszFilePath << std::endl;
                        file = c.c_str();
                    }
                    else
                    {
                        std::cerr << "Erreur lors de l'ouverture du fichier pour l'enregistrement." << std::endl;
                    }

                    CoTaskMemFree(pszFilePath);
                }

                pItem->Release();
            }
        }

        // Libération des ressources
        pFileSaveDialog->Release();
        CoUninitialize();

        return true;
    }
}