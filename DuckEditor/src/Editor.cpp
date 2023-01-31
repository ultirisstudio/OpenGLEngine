#include "Editor.h"
#include <glm/glm.hpp>

#include "imgui.h"

namespace DuckEngine
{
	Editor::Editor() : Layer("Editor"), m_ViewportFocus(false)
	{
		
	}

	void Editor::OnAttach()
	{
		m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0));

		cube.load("Models/cube.obj");
		sphere.load("Models/sphere.obj");
		plane.load("Models/plane.obj");

		m_Skybox = Renderer::CreateSkybox(cube);
		m_Texture = Renderer::CreateTexture("Textures/diffuse.png");
		m_TexturePNGIcon = Renderer::CreateTexture("Textures/texture_png.png");

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

		m_Skybox->Draw();

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
				if (ImGui::MenuItem("Ouvrir")) OpenExternalFile();
				ImGui::Separator();
				if (ImGui::MenuItem("Fermer")) DuckEngine::Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Objects"))
			{
				if (ImGui::MenuItem("Create cube")) AddGameObject(CUBE);
				if (ImGui::MenuItem("Create sphere")) AddGameObject(SPHERE);
				if (ImGui::MenuItem("Create plane")) AddGameObject(PLANE);
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
		if (ImGui::IsWindowHovered()) { m_ViewportFocus = true; }
		if (ImGui::IsWindowFocused()) { m_ViewportFocus = true; }
		else { m_ViewportFocus = false; }
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
				if (m_InspectorId == m_Objects[i]->GetID()) {
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
				bool opened = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, m_Objects[i]->GetName());
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
		ImGui::Image((void*)m_TexturePNGIcon->GetID(), ImVec2{ 64.0f, 64.0f });
		if (ImGui::IsItemClicked(1))
		{
			std::cout << "IMAGE" << std::endl;
		}
		ImGui::SameLine();
		ImGui::Image((void*)m_TexturePNGIcon->GetID(), ImVec2{ 64.0f, 64.0f });
		ImGui::End();

		ImGui::Begin("Open Resource Infos");
		ImGui::Text("Selected file: %s\nFile path: %s\nFile extension: %s\n", m_FileBrowser.GetInfos().m_SelectedFile.c_str(), m_FileBrowser.GetInfos().m_FilePath.c_str(), m_FileBrowser.GetInfos().m_FileExtension.c_str());
		ImGui::End();

		ImGui::End();
	}

	void Editor::OnEvent(Event& e)
	{
		if (m_ViewportFocus)
			m_Camera->OnEvent(e);
	}
	void Editor::AddGameObject(DEFAULT_OBJECT_TYPE type)
	{
		switch (type)
		{
		case CUBE:
			m_Objects.push_back(new GameObject(m_Objects.size(), "cube", cube, *m_Texture));
			break;
		case SPHERE:
			m_Objects.push_back(new GameObject(m_Objects.size(), "sphere", sphere, *m_Texture));
			break;
		case PLANE:
			m_Objects.push_back(new GameObject(m_Objects.size(), "plane", plane, *m_Texture));
			break;
		}
	}
	void Editor::OpenExternalFile()
	{
		m_FileBrowser.OpenFile();
		test.load(m_FileBrowser.GetInfos().m_FilePath);
		m_Objects.push_back(new GameObject(m_Objects.size(), "cube", test, *m_Texture));
	}
}