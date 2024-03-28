#include "ProjectManager.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <yaml-cpp/yaml.h>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <OpenGLEngine/Core/Application.h>
#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Scripting/ScriptEngine.h>

#include <Windows.h>

namespace OpenGLEngine
{
	ProjectManager::ProjectManager() : m_Properties(nullptr), m_CreateNewProjectDialog(false), m_OpenProjectDialog(false)
	{

	}

	ProjectManager::~ProjectManager()
	{

	}

	void ProjectManager::CreateNewProject()
	{
		m_OpenProjectDialog = false;
		m_CreateNewProjectDialog = true;
	}

	void ProjectManager::OpenProject()
	{
		m_CreateNewProjectDialog = false;
		m_OpenProjectDialog = true;
	}

	void ProjectManager::OnImGuiRender()
	{
		if (m_CreateNewProjectDialog)
		{
			ImGui::InputText("##project_name", &tempProjectName);
			ImGui::InputText("##project_path", &tempProjectPath);
			ImGui::SameLine();
			if (ImGui::Button("...", ImVec2(20, 20)))
			{
				tempProjectPath = m_FileBrowser.OpenFolder();
			}

			if (ImGui::Button("Create Project"))
			{
				std::string fullPath = tempProjectPath + std::string("\\") + tempProjectName;

				if (!std::filesystem::exists(fullPath))
				{
					m_Properties = new ProjectProperties();
					m_Properties->m_ProjectName = tempProjectName;
					m_Properties->m_ProjectPath = fullPath;

					std::filesystem::create_directory(m_Properties->m_ProjectPath);
					std::filesystem::create_directory(m_Properties->m_ProjectPath + std::string("\\Assets"));
					std::filesystem::create_directory(m_Properties->m_ProjectPath + std::string("\\Scripts"));

					CreateProjectFiles(m_Properties->m_ProjectName, m_Properties->m_ProjectPath);

					std::filesystem::path path(m_Properties->m_ProjectPath + "\\Scripts\\Build\\" + m_Properties->m_ProjectName + ".dll");
					ScriptEngine::SetAppAssemblyPath(path);

					YAML::Emitter out;
					out << YAML::BeginMap;
					out << YAML::Key << "Project" << YAML::Value << m_Properties->m_ProjectName;
					out << YAML::Key << "Path" << YAML::Value << m_Properties->m_ProjectPath;
					out << YAML::EndMap;

					std::ofstream fout(m_Properties->m_ProjectPath + std::string("\\" + m_Properties->m_ProjectName + std::string(".ultprj")));
					fout << out.c_str();

					Renderer::m_SceneData.m_ResourceManager.Reset();
				}

				m_CreateNewProjectDialog = false;
			}
		}

		if (m_OpenProjectDialog)
		{
			ImGui::InputText("##project_path", &tempProjectPath);
			ImGui::SameLine();
			if (ImGui::Button("..."))
			{
				tempProjectPath = m_FileBrowser.OpenFolder();
			}
			if (ImGui::Button("Open Project"))
			{
				OpenProjectFromPath(tempProjectPath);

				m_OpenProjectDialog = false;
			}
		}
	}

	void ProjectManager::CreateProjectFiles(const std::string& projectName, const std::string& projectPath)
	{
		std::string luaFileName = "premake5.lua";
		std::ofstream luaFile(projectPath + "\\Scripts\\" + luaFileName);
		std::string enginePath = std::filesystem::current_path().generic_string();

		luaFile << "include (\"" << enginePath << "/premake_customization/solution_items.lua\")\n";
		luaFile << "\n";
		luaFile << "Library = {}\n";
		luaFile << "Library[\"ScriptCore\"] = \"" << enginePath << "/Scripts/OpenGLEngine-ScriptCore.dll\"\n";
		luaFile << "\n";
		luaFile << "workspace \"" << projectName << "\"\n";
		luaFile << "    architecture \"x86_64\"\n";
		luaFile << "    startproject \"" << projectName << "\"\n";
		luaFile << "\n";
		luaFile << "    configurations\n";
		luaFile << "    {\n";
		luaFile << "        \"Debug\",\n";
		luaFile << "        \"Release\"\n";
		luaFile << "    }\n";
		luaFile << "    flags\n";
		luaFile << "    {\n";
		luaFile << "        \"MultiProcessorCompile\"\n";
		luaFile << "    }\n";
		luaFile << "\n";
		luaFile << "project \"" << projectName << "\"\n";
		luaFile << "	kind \"SharedLib\"\n";
		luaFile << "	language \"C#\"\n";
		luaFile << "	dotnetframework \"4.7.2\"\n";
		luaFile << "\n";
		luaFile << "	targetdir(\"%{wks.location}/Build\")\n";
		luaFile << "	objdir(\"%{wks.location}/Build/Intermediates\")\n";
		luaFile << "\n";
		luaFile << "	files\n";
		luaFile << "	{\n";
		luaFile << "	    \"Source/**.cs\",\n";
		luaFile << "	    \"Properties/**.cs\"\n";
		luaFile << "	}\n";
		luaFile << "\n";
		luaFile << "	links\n";
		luaFile << "	{\n";
		luaFile << "		\"%{Library.ScriptCore}\"\n";
		luaFile << "	}\n";
		luaFile << "	filter \"configurations:Debug\"\n";
		luaFile << "	    optimize \"Off\"\n";
		luaFile << "	    symbols \"Default\"\n";
		luaFile << "\n";
		luaFile << "	filter \"configurations:Release\"\n";
		luaFile << "	    optimize \"On\"\n";
		luaFile << "	    symbols \"Default\"\n";
		luaFile.close();

		std::string batFileName = "GenerateSolution.bat";
		std::ofstream batFile(projectPath + "\\" + batFileName);

		batFile << "@echo off\n";
		batFile << "pushd Scripts\n";
		batFile << "call " << enginePath << "\\premake\\premake5.exe vs2022\n";
		batFile << "popd\n";
		batFile << "PAUSE\n";
		batFile.close();
	}

	void ProjectManager::OpenProjectFromPath(const std::string& projectPath)
	{
		std::string projectName = std::filesystem::path(projectPath + "\\").parent_path().filename().string();
		std::string projectFilePath = projectPath + "\\" + projectName + ".ultprj";

		bool projectExists = std::filesystem::exists(projectFilePath);

		if (projectExists)
		{
			std::ifstream stream(projectFilePath);
			std::stringstream strStream;
			strStream << stream.rdbuf();

			YAML::Node data = YAML::Load(strStream.str());

			m_Properties = new ProjectProperties();
			m_Properties->m_ProjectName = data["Project"].as<std::string>();
			m_Properties->m_ProjectPath = data["Path"].as<std::string>();

			std::string command;
			command += std::filesystem::current_path().generic_string() + "\\OpenGLEditor.exe";
			//command += " --projectName=" + m_Properties->m_ProjectName;
			//command += " --projectPath=" + m_Properties->m_ProjectPath;

			//std::cout << command << std::endl;

			//run_process(command.c_str());


			STARTUPINFO StartupInfo;
			PROCESS_INFORMATION ProcessInfo;

			ZeroMemory(&StartupInfo, sizeof(StartupInfo));
			StartupInfo.cb = sizeof(LPSTARTUPINFO);
			ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

			CreateProcessW(LPCWSTR(command.c_str()),
				NULL, NULL, NULL,
				NULL, NULL, NULL, NULL,
				&StartupInfo,
				&ProcessInfo
			);

			//Application::Get().Close();

			// Lancer une nouvelle application

			//system(command.c_str());

			/*const char* pszCommand = command.c_str();
			const char* pszTitle = std::string(std::filesystem::current_path().generic_string() + "\\OpenGLEditor.exe").c_str();

			STARTUPINFO StartInfo;
			PROCESS_INFORMATION ProcessInfo;

			memset(&StartInfo, 0, sizeof(StartInfo));

			StartInfo.cb = sizeof(StartInfo);
			StartInfo.lpTitle = (pszTitle) ? (char*)pszTitle : (char*)pszCommand;

			StartInfo.wShowWindow = SW_NORMAL;
			StartInfo.dwFlags |= STARTF_USESHOWWINDOW;

			CreateProcess(0, (char*)pszCommand,
				0, 0, TRUE,
				CREATE_NEW_PROCESS_GROUP, 0, 0,
				&StartInfo, &ProcessInfo);*/
		}
	}

	void ProjectManager::run_process(const char* path)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		bool ret = CreateProcess(
			NULL,          // No module name (use command line)
			LPWSTR(path),  // Command line
			NULL,          // Process handle not inheritable
			NULL,          // Thread handle not inheritable
			false,         // Set handle inheritance to FALSE
			0,             // No creation flags
			NULL,          // Use parent's environment block
			NULL,          // Use parent's starting directory 
			&si,           // Pointer to STARTUPINFO structure
			&pi            // Pointer to PROCESS_INFORMATION structure
		);

		if (!ret) {
			printf("Error");
			abort();
		}

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}