#include <OpenGLEngine.h>
#include <OpenGLEngine/Core/EntryPoint.h>
#include "Editor.h"

namespace OpenGLEngine
{
	class DuckEditor : public Application
	{
	public:
		DuckEditor() : Application("Duck Editor")
		{
			PushLayer(new Editor());
		}

		~DuckEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new DuckEditor();
	}
}