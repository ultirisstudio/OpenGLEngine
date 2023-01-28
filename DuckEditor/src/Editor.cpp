#include "Editor.h"
#include <glm/glm.hpp>

#include "imgui.h"

namespace DuckEngine
{
	Editor::Editor() : Layer("Editor")
	{
		
	}

	void Editor::OnAttach()
	{
		m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0));

		renderModel = Renderer::CreateRenderModel("src/Models/cube.obj");
		renderModel_2 = Renderer::CreateRenderModel("src/Models/cube.obj");

		m_frameBuffer = new Framebuffer(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		m_frameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		m_frameBuffer->setDepthAttachment();
		m_frameBuffer->Create();

		m_Position[0] = 0.0f;
		m_Position[1] = 0.0f;
		m_Position[2] = 0.0f;

		m_Rotation[0] = 0.0f;
		m_Rotation[1] = 0.0f;
		m_Rotation[2] = 0.0f;

		m_Scale[0] = 1.0f;
		m_Scale[1] = 1.0f;
		m_Scale[2] = 1.0f;
	}

	void Editor::OnDetach()
	{
		
	}

	void Editor::OnUpdate()
	{
		m_frameBuffer->bind();

		Renderer::Clear();
		Renderer::ClearColor(glm::vec4(0.5f, 0.5f, .5f, 1.0f));

		m_Camera->Update();

		Renderer::BeginScene(m_Camera);

		renderModel->draw();
		renderModel_2->draw();

		renderModel->setPosition(glm::vec3(m_Position[0], m_Position[1], m_Position[2]));
		renderModel->setRotation(glm::vec3(m_Rotation[0], m_Rotation[1], m_Rotation[2]));
		renderModel->setScale(glm::vec3(m_Scale[0], m_Scale[1], m_Scale[2]));

		float t = glfwGetTime();
		renderModel->rotate(glm::vec3(t * 50.0f));

		Renderer::EndScene();

		m_frameBuffer->unbind();
	}

	void Editor::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Fichier"))
			{
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();
				if (ImGui::MenuItem("Fermer")) DuckEngine::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			if (m_frameBuffer)
				delete m_frameBuffer;
			m_frameBuffer = new Framebuffer((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_frameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
			m_frameBuffer->setDepthAttachment();
			m_frameBuffer->Create();

			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_Camera->OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = m_frameBuffer->getColorAttachment(0);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Cube");
		ImGui::Text("Transform");
		ImGui::SliderFloat3("Position", m_Position, -100.0f, 100.0f);
		ImGui::Text("Rotation");
		ImGui::SliderFloat3("Rotation", m_Rotation, 0.0f, 180.0f);
		ImGui::Text("Scale");
		ImGui::SliderFloat3("Scale", m_Scale, 0.1f, 100.0f);
		ImGui::End();

		ImGui::Begin("Statistiques");
		ImGui::End();

		ImGui::End();
	}

	void Editor::OnEvent(Event& e)
	{
		m_Camera->OnEvent(e);
	}
}