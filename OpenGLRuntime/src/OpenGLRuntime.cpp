#include <OpenGLEngine.h>
#include <OpenGLEngine/Core/EntryPoint.h>

#include "Runtime/Runtime.h"

namespace OpenGLEngine
{
	class OpenGLRuntime : public Application
	{
	public:
		OpenGLRuntime(const ApplicationSpecification& spec) : Application(spec)
		{
			PushLayer(new Runtime());
		}

		~OpenGLRuntime()
		{

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Runtime";
		spec.CommandLineArgs = args;

		return new OpenGLRuntime(spec);
	}
}