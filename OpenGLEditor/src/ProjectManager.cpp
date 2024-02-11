#include "ProjectManager.h"

#include <iostream>

#include <imgui/imgui.h>

namespace OpenGLEngine
{
	ProjectManager::ProjectManager() : m_ProjectPath(""), m_ProjectName(""), m_CreateNewProjectDialog(false)
	{

	}

	ProjectManager::ProjectManager(std::string projectPath) : m_ProjectPath(projectPath), m_ProjectName(""), m_CreateNewProjectDialog(false)
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
			/*ImGui::Begin("Create new project");
			ImGui::InputText("Project name", m_ProjectName, 256);
			ImGui::InputText("Project path", m_ProjectPath, 256);
			if (ImGui::Button("Create project"))
			{
				std::cout << "Creating new project: " << m_ProjectName << " at " << m_ProjectPath << std::endl;
				m_CreateNewProjectDialog = false;
			}
			ImGui::End();*/

			ImGui::SetNextWindowSize(ImVec2(260, 180));
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGui::Begin("test", &m_CreateNewProjectDialog);
			{
				ImGui::SetCursorPos(ImVec2(20, 40));
				ImGui::InputText("##Username", m_ProjectName.data(), 254);

				ImGui::SetCursorPos(ImVec2(20, 80));
				ImGui::InputText("##Password", m_ProjectPath.data(), 254);
				ImGui::SameLine();
				if (ImGui::Button("X"))
				{
					m_ProjectPath = m_FileBrowser.OpenFolder();
				}

				ImGui::SetCursorPos(ImVec2(60, 120));
				if (ImGui::Button("Create Project", ImVec2(100, 40)))
				{
					std::cout << "Creating new project: " << m_ProjectName << " at " << m_ProjectPath << std::endl;
					m_CreateNewProjectDialog = false;
				}
			}
			ImGui::End();
		}
	}
}