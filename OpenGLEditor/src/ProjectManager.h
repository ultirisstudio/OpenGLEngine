#pragma once

#include <string>

#include "FileBrowser.h"

#include "Panels/ContentBrowserPanel.h"

namespace OpenGLEngine
{
	struct ProjectProperties
	{
		std::string m_ProjectPath;
		std::string m_ProjectName;
	};

	class ProjectManager
	{
	public:
		ProjectManager();
		~ProjectManager();

		void CreateNewProject();
		void OpenProject();
		void SaveProject();
		void SaveProjectAs();
		void CloseProject();

		ProjectProperties* GetProjectProperties() { return m_Properties; }

		void OnImGuiRender(ContentBrowserPanel& contentBrowserPanel);
	private:
		ProjectProperties* m_Properties;
		FileBrowser m_FileBrowser;

		std::string tempProjectName = "";
		std::string tempProjectPath = "";

		bool m_CreateNewProjectDialog;
		bool m_OpenProjectDialog;
	};
}
