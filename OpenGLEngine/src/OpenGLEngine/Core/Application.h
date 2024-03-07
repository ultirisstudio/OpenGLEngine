#pragma once

#include "OpenGLEngine/Core/Core.h"
#include "OpenGLEngine/Events/Event.h"
#include "OpenGLEngine/Events/ApplicationEvent.h"
#include "OpenGLEngine/Core/Window.h"
#include "OpenGLEngine/Core/LayerManager.h"
#include "OpenGLEngine/Core/Layer.h"
#include "OpenGLEngine/ImGui/ImGuiLayer.h"
#include "OpenGLEngine/Renderer/Renderer.h"

int main(int argc, char** argv);

namespace OpenGLEngine
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			if (index >= Count)
				return nullptr;

			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Duck Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void OnEvent(Event& e);

		void MaximizeWindow(bool value);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

		void Close();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseMove(MouseMovedEvent& e);
	private:
		ApplicationSpecification m_Specification;

		bool m_Running = true;
		bool m_Minimized = false;

		double previousTime = Renderer::GetTime();
		int frameCount = 0;

		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerManager m_LayerManager;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
