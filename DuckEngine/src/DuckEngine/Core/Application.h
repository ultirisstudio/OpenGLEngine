#pragma once

#include "DuckEngine/Core/Core.h"
#include "DuckEngine/Events/Event.h"
#include "DuckEngine/Events/ApplicationEvent.h"
#include "DuckEngine/Core/Window.h"

namespace DuckEngine
{
	class Application
	{
	public:
		Application(const std::string& name = "Duck Application");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		inline static Application& Get() { return *s_Instance; }
	private:
			bool OnWindowClose(WindowCloseEvent& e);
			bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
