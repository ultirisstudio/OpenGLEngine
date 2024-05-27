#include <OpenGLEngine.h>
#include <OpenGLEngine/Core/EntryPoint.h>

#include "Editor/Editor.h"
#include "Launcher/Launcher.h"
#include "yaml-cpp/yaml.h"
#include <fstream>

namespace OpenGLEngine
{
	class OpenGLEditor : public Application
	{
	public:
		OpenGLEditor(const ApplicationSpecification& spec) : Application(spec)
		{

			//Check if config.yaml exists if it does then load the editor with the project name and path
			//If it doesn't exist then load the launcher
			if (std::filesystem::exists("config.yaml"))
			{
				YAML::Node config = YAML::LoadFile("config.yaml");
				if (config["projectName"] && config["projectPath"])
				{
					EditorSpecification editorSpec;
					editorSpec.EngineExecutablePath = spec.CommandLineArgs[0];
					editorSpec.ProjectName = config["projectName"].as<std::string>();
					editorSpec.ProjectPath = config["projectPath"].as<std::string>();

					PushLayer(new Editor(editorSpec));
				}
				else
				{
					PushLayer(new Launcher());
				}
			}
			else
			{
				// create config file with empty values
				YAML::Node config;
				config["projectName"] = "null";
				config["projectPath"] = "null";
				std::ofstream fout("config.yaml");
				fout << config;
				fout.close();

				PushLayer(new Launcher());
			}

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