#include "ProjectManager.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <yaml-cpp/yaml.h>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <OpenGLEngine/Renderer/Renderer.h>

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

						contentBrowserPanel.ChangeProjectPath(m_Properties->m_ProjectPath);
					}
					m_OpenProjectDialog = false;
				}
				ImGui::EndPopup();
			}
		}
	}
}