#include "ProjectManager.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <yaml-cpp/yaml.h>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Scripting/ScriptEngine.h>

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
		m_CreateNewProjectDialog = true;
	}

	void ProjectManager::OpenProject()
	{
		m_OpenProjectDialog = true;
	}

	void ProjectManager::SaveProject()
	{

	}

	void ProjectManager::SaveProjectAs()
	{

	}

	void ProjectManager::CloseProject()
	{
		m_Properties = nullptr;
		Renderer::m_SceneData.m_ResourceManager.Reset();
	}

	void ProjectManager::OnImGuiRender(ContentBrowserPanel& contentBrowserPanel)
	{
		if (m_CreateNewProjectDialog)
		{
			ImGui::SetNextWindowSize(ImVec2(260, 180));
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGui::OpenPopup("Create new project");

			if (ImGui::BeginPopupModal("Create new project")) {
				ImGui::SetCursorPos(ImVec2(20, 40));
				ImGui::InputText("##project_name", &tempProjectName);

				ImGui::SetCursorPos(ImVec2(20, 80));
				ImGui::InputText("##project_path", &tempProjectPath);
				ImGui::SameLine();
				if (ImGui::Button("...", ImVec2(20, 20)))
				{
					tempProjectPath = m_FileBrowser.OpenFolder();
				}

				ImGui::SetCursorPos(ImVec2(20, 120));
				if (ImGui::Button("Create Project", ImVec2(110, 30)))
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

						CreateSolutionFile(m_Properties->m_ProjectName, m_Properties->m_ProjectPath);
						CreateProjectFile(m_Properties->m_ProjectName, m_Properties->m_ProjectPath);

						std::filesystem::path path(m_Properties->m_ProjectPath + "\\Scripts\\Build\\Release\\net8.0\\" + m_Properties->m_ProjectName + ".dll");
						ScriptEngine::SetAppAssemblyPath(path);

						YAML::Emitter out;
						out << YAML::BeginMap;
						out << YAML::Key << "Project" << YAML::Value << m_Properties->m_ProjectName;
						out << YAML::Key << "Path" << YAML::Value << m_Properties->m_ProjectPath;
						out << YAML::EndMap;

						std::ofstream fout(m_Properties->m_ProjectPath + std::string("\\" + m_Properties->m_ProjectName + std::string(".ultprj")));
						fout << out.c_str();

						Renderer::m_SceneData.m_ResourceManager.Reset();
						contentBrowserPanel.ChangeProjectPath(m_Properties->m_ProjectPath);
					}

					m_CreateNewProjectDialog = false;
				}
				ImGui::EndPopup();
			}
		}

		if (m_OpenProjectDialog)
		{
			ImGui::SetNextWindowSize(ImVec2(260, 180));
			ImGui::SetNextWindowBgAlpha(1.0f);
			ImGui::OpenPopup("Open project");

			if (ImGui::BeginPopupModal("Open project"))
			{
				ImGui::SetCursorPos(ImVec2(20, 80));
				ImGui::InputText("##project_path", &tempProjectPath);
				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					tempProjectPath = m_FileBrowser.OpenFolder();
				}
				ImGui::SetCursorPos(ImVec2(60, 120));
				if (ImGui::Button("Open Project", ImVec2(100, 40)))
				{
					tempProjectName = std::filesystem::path(tempProjectPath + "\\").parent_path().filename().string();
					std::string projectFilePath = tempProjectPath + "\\" + tempProjectName + ".ultprj";

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

						std::filesystem::path path(m_Properties->m_ProjectPath + "\\Scripts\\Build\\Release\\net8.0\\" + m_Properties->m_ProjectName + ".dll");
						ScriptEngine::SetAppAssemblyPath(path);

						contentBrowserPanel.ChangeProjectPath(m_Properties->m_ProjectPath);
					}
					m_OpenProjectDialog = false;
				}
				ImGui::EndPopup();
			}
		}
	}
	void ProjectManager::CreateSolutionFile(const std::string& projectName, const std::string& projectPath)
	{
		std::string solutionFileName = projectName + ".sln";
		std::ofstream solutionFile(projectPath + "\\" + solutionFileName);

		if (solutionFile.is_open())
		{
			solutionFile << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
			solutionFile << "# Visual Studio 17\n";
			solutionFile << "VisualStudioVersion = 17.8.34316.72\n";
			solutionFile << "MinimumVisualStudioVersion = 10.0.40219.1\n";
			solutionFile << "Project(\"{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}\") = \"" << projectName << "\", \"" << "Scripts" << "\\" << projectName << ".csproj\", \"{B14D82A7-54D2-4621-AF92-B9EC432CC8A2}\"\n";
			solutionFile << "EndProject\n";
			solutionFile << "Global\n";
			solutionFile << "	GlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
			solutionFile << "		Debug|Any CPU = Debug|Any CPU\n";
			solutionFile << "		Release|Any CPU = Release|Any CPU\n";
			solutionFile << "	EndGlobalSection\n";
			solutionFile << "	GlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
			solutionFile << "		{B14D82A7-54D2-4621-AF92-B9EC432CC8A2}.Debug|Any CPU.ActiveCfg = Debug|Any CPU\n";
			solutionFile << "		{B14D82A7-54D2-4621-AF92-B9EC432CC8A2}.Debug|Any CPU.Build.0 = Debug|Any CPU\n";
			solutionFile << "		{B14D82A7-54D2-4621-AF92-B9EC432CC8A2}.Release|Any CPU.ActiveCfg = Release|Any CPU\n";
			solutionFile << "		{B14D82A7-54D2-4621-AF92-B9EC432CC8A2}.Release|Any CPU.Build.0 = Release|Any CPU\n";
			solutionFile << "	EndGlobalSection\n";
			solutionFile << "	GlobalSection(SolutionProperties) = preSolution\n";
			solutionFile << "		HideSolutionNode = FALSE\n";
			solutionFile << "	EndGlobalSection\n";
			solutionFile << "	GlobalSection(ExtensibilityGlobals) = postSolution\n";
			solutionFile << "		SolutionGuid = {819439FB-134B-4952-88A4-631CA50640E8}\n";
			solutionFile << "	EndGlobalSection\n";
			solutionFile << "EndGlobal\n";
		}

		solutionFile.close();
	}

	void ProjectManager::CreateProjectFile(const std::string& projectName, const std::string& projectPath)
	{
		std::string projectFileName = projectName + ".csproj";
		std::ofstream projectFile(projectPath + "\\Scripts\\" + projectFileName);
		std::string enginePath = std::filesystem::current_path().generic_string();

		if (projectFile.is_open())
		{
			projectFile << "<Project Sdk=\"Microsoft.NET.Sdk\">\n";
			projectFile << "  <PropertyGroup>\n";
			projectFile << "    <TargetFramework>net8.0</TargetFramework>\n";
			projectFile << "    <ImplicitUsings>enable</ImplicitUsings>\n";
			projectFile << "    <Nullable>enable</Nullable>\n";
			projectFile << "    <BaseOutputPath>Build</BaseOutputPath>\n";
			projectFile << "    <BaseIntermediateOutputPath>Build\\Intermediates</BaseIntermediateOutputPath>\n";
			projectFile << "  </PropertyGroup>\n";
			projectFile << "  <ItemGroup>\n";
			projectFile << "    <Reference Include=\"OpenGLEngine-ScriptCore\">\n";
			projectFile << "      <HintPath>" << enginePath << "\\Scripts\\OpenGLEngine-ScriptCore.dll</HintPath>\n";
			projectFile << "    </Reference>\n";
			projectFile << "  </ItemGroup>\n";
			projectFile << "</Project>\n";
		}

		projectFile.close();
	}
}