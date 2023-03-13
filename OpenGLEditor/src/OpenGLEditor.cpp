#include <OpenGLEngine.h>
#include <OpenGLEngine/Core/EntryPoint.h>
#include "Editor.h"

namespace OpenGLEngine
{
	class OpenGLEditor : public Application
	{
	public:
		OpenGLEditor() : Application("Duck Editor")
		{
			PushLayer(new Editor());
		}

		~OpenGLEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new OpenGLEditor();
	}
}