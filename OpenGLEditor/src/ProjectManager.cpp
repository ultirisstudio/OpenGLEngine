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
	ProjectManager::ProjectManager() : m_ProjectPath(""), m_ProjectName(""), m_CreateNewProjectDialog(false)
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

	}

	void ProjectManager::OnImGuiRender()
	{
		if (m_CreateNewProjectDialog)
		{
			ImGui::SetNextWindowSize(ImVec2(260, 180));
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGui::OpenPopup("Create new project");

			if (ImGui::BeginPopupModal("Create new project")) {
				ImGui::SetCursorPos(ImVec2(20, 40));
				ImGui::InputText("##project_name", &m_ProjectName);

				ImGui::SetCursorPos(ImVec2(20, 80));
				ImGui::InputText("##project_path", &m_ProjectPath);
				ImGui::SameLine();
				if (ImGui::Button("...", ImVec2(20, 20)))
				{
					m_ProjectPath = m_FileBrowser.OpenFolder();
				}

				ImGui::SetCursorPos(ImVec2(20, 120));
				if (ImGui::Button("Create Project", ImVec2(110, 30)))
				{
					std::cout << "Creating new project: " << m_ProjectName << " at " << m_ProjectPath << std::endl;

					std::string fullPath = m_ProjectPath + std::string("\\") + m_ProjectName;

					if (!std::filesystem::exists(m_ProjectPath + m_ProjectName))
					{
						std::filesystem::create_directory(fullPath);
						std::filesystem::create_directory(fullPath + std::string("\\Assets"));

						YAML::Emitter out;
						out << YAML::BeginMap;
						out << YAML::Key << "Project" << YAML::Value << m_ProjectName;
						out << YAML::Key << "Path" << YAML::Value << m_ProjectPath;
						out << YAML::EndMap;

						std::ofstream fout(fullPath + std::string("\\" + m_ProjectName + std::string(".ultprj")));
						fout << out.c_str();
					}

					Renderer::m_SceneData.m_ResourceManager.Reset();
					m_CreateNewProjectDialog = false;
				}
				ImGui::EndPopup();
			}

			/*ImGui::Begin("test", &m_CreateNewProjectDialog);
			{
				ImGui::SetCursorPos(ImVec2(20, 40));
				ImGui::InputText("##project_name", &m_ProjectName);

				ImGui::SetCursorPos(ImVec2(20, 80));
				ImGui::InputText("##project_path", &m_ProjectPath);
				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					m_ProjectPath = m_FileBrowser.OpenFolder();
				}

				ImGui::SetCursorPos(ImVec2(60, 120));
				if (ImGui::Button("Create Project", ImVec2(100, 40)))
				{
					std::cout << "Creating new project: " << m_ProjectName << " at " << m_ProjectPath << std::endl;
					Renderer::m_SceneData.m_ResourceManager.Reset();
					m_CreateNewProjectDialog = false;
				}
			}
			ImGui::End();*/
		}

		if (m_OpenProjectDialog)
		{
			ImGui::SetNextWindowSize(ImVec2(260, 180));
			ImGui::SetNextWindowBgAlpha(1.0f);
			ImGui::Begin("test", &m_OpenProjectDialog);
			{
				ImGui::SetCursorPos(ImVec2(20, 80));
				ImGui::InputText("##project_path", &m_ProjectPath);
				ImGui::SameLine();
				if (ImGui::Button("X"))
				{
					m_ProjectPath = m_FileBrowser.OpenFolder();
				}
				ImGui::SetCursorPos(ImVec2(60, 120));
				if (ImGui::Button("Open Project", ImVec2(100, 40)))
				{
					std::cout << "Opening project: " << m_ProjectName << " at " << m_ProjectPath << std::endl;
					m_OpenProjectDialog = false;
				}
			}
			ImGui::End();
		}
	}
}