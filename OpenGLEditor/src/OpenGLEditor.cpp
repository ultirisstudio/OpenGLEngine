#include <OpenGLEngine.h>
#include <OpenGLEngine/Core/EntryPoint.h>

#include "Editor/Editor.h"
#include "Launcher/Launcher.h"

namespace OpenGLEngine
{
	class OpenGLEditor : public Application
	{
	public:
		OpenGLEditor(const ApplicationSpecification& spec) : Application(spec)
		{
			//PushLayer(new Launcher());

			EditorSpecification editorSpec;
			editorSpec.EngineExecutablePath = spec.CommandLineArgs[0];
			editorSpec.ProjectName = "CallOf";
			editorSpec.ProjectPath = "C:\\Users\\rouff\\Documents\\Ultiris Projects\\CallOf";

			PushLayer(new Editor(editorSpec));

			/*bool haveProjectName = false;
			bool haveProjectPath = false;

			std::string projectName;
			std::string projectPath;

			for (int i = 0; i < spec.CommandLineArgs.Count; i++)
			{
				std::string temp(spec.CommandLineArgs[i]);

				size_t pos = 0;
				std::string token;

				if ((pos = temp.find("=")) != std::string::npos)
				{
					std::string left = temp.substr(0, pos);
					temp.erase(0, pos + std::string("=").length());
					std::string right = temp;
					
					if (left == "--projectName")
					{
						haveProjectName = true;
						projectName = right;
					}

					if (left == "--projectPath")
					{
						haveProjectPath = true;
						projectPath = right;
					}
				}
			}

			if (haveProjectName && haveProjectPath)
			{
				EditorSpecification editorSpec;
				editorSpec.EngineExecutablePath = spec.CommandLineArgs[0];
				editorSpec.ProjectName = projectName;
				editorSpec.ProjectPath = projectPath;

				PushLayer(new Editor(editorSpec));
			}
			else
			{
				PushLayer(new Launcher());
			}*/
		}

		~OpenGLEditor()
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Duck Editor";
		spec.CommandLineArgs = args;

		return new OpenGLEditor(spec);
	}
}