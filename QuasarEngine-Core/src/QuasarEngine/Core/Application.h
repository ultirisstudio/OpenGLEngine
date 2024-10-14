#pragma once

#include "QuasarEngine/Core/Core.h"
#include "QuasarEngine/Events/Event.h"
#include "QuasarEngine/Events/ApplicationEvent.h"
#include "QuasarEngine/Core/Window.h"
#include "QuasarEngine/Core/LayerManager.h"
#include "QuasarEngine/Core/Layer.h"
#include "QuasarEngine/ImGui/ImGuiLayer.h"
#include "QuasarEngine/Renderer/Renderer.h"
#include <QuasarEngine/Events/MouseEvent.h>

int main(int argc, char** argv);

namespace QuasarEngine
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

		float deltaTime = 0;
		float lastFrame = 0;

		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerManager m_LayerManager;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
