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

		model.load("src/Models/cube.obj");;

		m_frameBuffer = new Framebuffer(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		m_frameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		m_frameBuffer->setDepthAttachment();
		m_frameBuffer->Create();
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

		for (GameObject* object : m_Objects) {
			object->Render();
			object->Draw();
		}

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
				if (ImGui::MenuItem("Ouvrir"));
				ImGui::Separator();
				if (ImGui::MenuItem("Fermer")) DuckEngine::Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Objects"))
			{
				if (ImGui::MenuItem("Create cube")) AddGameObject();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
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

		ImGui::Begin("Inspector");

		if (m_InspectorId >= 0) {
			for (int i = 0; i < m_Objects.size(); i++) {
				if (m_InspectorId == m_Objects[i]->m_Id) {
					m_Objects[i]->DrawInspector();
				}
			}
		}

		ImGui::End();

		ImGui::Begin("Scene");

		static int selection_mask = 0x02;
		int node_clicked = -1;

		if (ImGui::TreeNode("Objects"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1);
			for (int i = 0; i < m_Objects.size(); i++)
			{
				ImGuiTreeNodeFlags flags = ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
				bool opened = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, m_Objects[i]->m_Name);
				if (ImGui::IsItemClicked())
				{
					node_clicked = i;
					m_InspectorId = i;
				}

				if (opened)
				{
					ImGui::TreePop();
				}

				if (node_clicked != -1)
				{
					if (ImGui::GetIO().KeyCtrl)
						selection_mask ^= (1 << node_clicked); 
					else
						selection_mask = (1 << node_clicked);
				}
			}
			ImGui::TreePop();
			ImGui::PopStyleVar();
		}

		ImGui::End();

		ImGui::Begin("Contenu");

		ImGui::End();

		ImGui::End();
	}

	void Editor::OnEvent(Event& e)
	{
		m_Camera->OnEvent(e);
	}
	void Editor::AddGameObject()
	{
		m_Objects.push_back(new GameObject(0 + m_Objects.size(), model));
	}
}