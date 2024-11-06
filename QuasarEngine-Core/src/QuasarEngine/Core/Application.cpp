#include "qepch.h"

#include <GLFW/glfw3.h>

#include "Application.h"
#include "QuasarEngine/Renderer/Renderer.h"
#include "QuasarEngine/Core/Input.h"

#include <Windows.h>

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace QuasarEngine
{
	Application* Application::s_Instance = nullptr;
	
	Application::Application(const ApplicationSpecification& specification) : m_Specification(specification)
	{
		s_Instance = this;

		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_Window->SetVSync(false);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Application::OnMouseMove, this, std::placeholders::_1));

		for (Layer* layer : m_LayerManager)
			layer->OnEvent(e);
	}

	void Application::MaximizeWindow(bool value)
	{
		if (value)
			glfwMaximizeWindow(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerManager.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerManager.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerManager)
					layer->OnUpdate(deltaTime);
			}

			double currentTime = Renderer::GetTime();
			if (currentTime - last_time >= (1.0 / 120.0))
			{
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerManager)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();

				//m_Window->OnUpdate();
				m_Window->SwapBuffers();
				m_Window->PollEvents();

				last_time += (1.0 / 120.0);
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnMouseMove(MouseMovedEvent& e) {
		Window::WindowData& data = *(Window::WindowData*)glfwGetWindowUserPointer(reinterpret_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		data.MousePos.x = e.GetX();
		data.MousePos.y = e.GetY();
		return false;
	}
}
