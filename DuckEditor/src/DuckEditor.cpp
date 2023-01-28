#include <DuckEngine.h>
#include <DuckEngine/Core/EntryPoint.h>
#include "Editor.h"

namespace DuckEngine
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