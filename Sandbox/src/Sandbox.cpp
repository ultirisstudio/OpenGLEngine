#include <DuckEngine.h>
#include <DuckEngine/Core/EntryPoint.h>

namespace DuckEngine
{
	class UltirisEditor : public Application
	{
	public:
		UltirisEditor() : Application("Duck Editor")
		{

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