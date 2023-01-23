#include "depch.h"
#include "Application.h"
#include "DuckEngine/Renderer/Renderer.h"

namespace DuckEngine
{
	Application* Application::s_Instance = nullptr;
	
	Application::Application(const std::string& name)
	{
		s_Instance = this;

		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_Window->SetVSync(false);

		m_ImGuiLayer = new ImGuiLayer();
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		m_Layer->OnEvent(e);
	}

	void Application::SetLayer(Layer* layer)
	{
		m_Layer = layer;
		m_Layer->Init();
		m_ImGuiLayer->Init();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Layer->OnUpdate();
			m_ImGuiLayer->OnImGuiRender();
			m_Window->OnUpdate();
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
		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}
