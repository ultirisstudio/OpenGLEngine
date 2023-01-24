#include <DuckEngine.h>
#include <DuckEngine/Core/EntryPoint.h>
#include "Editor.h"

namespace DuckEngine
{
	class UltirisEditor : public Application
	{
	public:
		UltirisEditor() : Application("Duck Editor")
		{
			SetLayer(new Editor());
		}

		~UltirisEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new UltirisEditor();
	}
}