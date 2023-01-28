#pragma once

#include "DuckEngine/Core/Core.h"
#include "DuckEngine/Events/Event.h"
#include "DuckEngine/Events/ApplicationEvent.h"
#include "DuckEngine/Core/Window.h"
#include "DuckEngine/Core/LayerManager.h"
#include "DuckEngine/Core/Layer.h"
#include "DuckEngine/ImGui/ImGuiLayer.h"
#include "DuckEngine/Renderer/Renderer.h"

namespace DuckEngine
{
	class Application
	{
	public:
		Application(const std::string& name = "Duck Application");
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void OnEvent(Event& e);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

		void Close();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseMove(MouseMovedEvent& e);
	private:
		bool m_Running = true;
		bool m_Minimized = false;

		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerManager m_LayerManager;
	};

	Application* CreateApplication();
}
