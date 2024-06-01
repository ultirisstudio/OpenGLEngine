#pragma once

#include <string>

#include "../Utils/FileBrowser.h"

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

		void OpenProjectFromPath(const std::string& projectPath);

		ProjectProperties* GetProjectProperties() { return m_Properties; }

		void OnImGuiRender();
	private:
		void CreateProjectFiles(const std::string& projectName, const std::string& projectPath);
	private:
		ProjectProperties* m_Properties;
		FileBrowser m_FileBrowser;

		std::string tempProjectName = "";
		std::string tempProjectPath = std::string(getenv("USERPROFILE")) + "\\Documents\\Ultiris Projects";

		bool m_CreateNewProjectDialog;
		bool m_OpenProjectDialog;
	};
}
