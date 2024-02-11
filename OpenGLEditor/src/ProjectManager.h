#pragma once

#include <string>

#include <OpenGLEngine/Tools/FileBrowser.h>

namespace OpenGLEngine
{
	class ProjectManager
	{
	public:
		ProjectManager();
		ProjectManager(std::string projectPath);
		~ProjectManager();

		void CreateNewProject();
		void OpenProject();
		void SaveProject();
		void SaveProjectAs();
		void CloseProject();

		void OnImGuiRender();
	private:
		std::string m_ProjectPath;
		std::string m_ProjectName;
		FileBrowser m_FileBrowser;

		bool m_CreateNewProjectDialog;
	};
}
