#include "Editor.h"
#include <glm/glm.hpp>

#include "imgui.h"

namespace DuckEngine
{
	Editor::Editor() : Layer("Editor"), m_ViewportFocus(false), m_ContentBrowserPanel(), m_EntityPropertiePanel(), m_SelectedEntity(nullptr)
	{
		
	}

	void Editor::OnAttach()
	{
		m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0));

		cube = Renderer::CreateModel("Assets/Models/cube.obj");
		sphere = Renderer::CreateModel("Assets/Models/sphere.obj");
		plane = Renderer::CreateModel("Assets/Models/plane.obj");

		//m_Skybox = Renderer::CreateSkybox(*cube);
		m_Texture = Renderer::CreateTexture("Assets/Textures/diffuse.png");

		m_frameBuffer = std::make_shared<Framebuffer>(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		m_frameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		m_frameBuffer->setDepthAttachment();
		m_frameBuffer->Create();

		Entity* m_Entity = new Entity();
		m_Entity->SetId(m_Entities.size());
		m_Entity->SetName("Entity");
		m_Entity->AddComponent<TransformComponent>();
		m_Entity->AddComponent<ModelComponent>("Assets/Models/cube.obj");
		m_Entity->AddComponent<MaterialComponent>("Assets/Textures/diffuse.png");
		m_Entity->AddComponent<RenderComponent>();
		m_Entities.push_back(m_Entity);
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

		if (m_ViewportFocus)
			m_Camera->m_CameraFocus = true;
		else
			m_Camera->m_CameraFocus = false;

		Renderer::BeginScene(m_Camera.get());

		for (Entity* entity : m_Entities) {
			entity->Draw();
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
				if (ImGui::MenuItem("Ouvrir")) OpenExternalFile();
				ImGui::Separator();
				if (ImGui::MenuItem("Fermer"))
					DuckEngine::Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Create new GameObject"))
				{
					Entity* temp = new Entity();
					temp->SetId(m_Entities.size());
					temp->SetName("GameObject");
					m_Entities.push_back(temp);
				}

				if (ImGui::MenuItem("Create new Cube")) AddGameObject(CUBE);
				if (ImGui::MenuItem("Create new Sphere")) AddGameObject(SPHERE);
				if (ImGui::MenuItem("Create new Plane")) AddGameObject(PLANE);
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
		if (ImGui::IsWindowHovered()) { m_ViewportFocus = true; } else { m_ViewportFocus = false; }

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_frameBuffer = std::make_shared<Framebuffer>((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_frameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
			m_frameBuffer->setDepthAttachment();
			m_frameBuffer->Create();

			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_Camera->OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = m_frameBuffer->getColorAttachment(0);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path file = path;
				AddGameObject(file.string());
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();

		ImGui::PopStyleVar();

		ImGui::Begin("Inspector");

		if (m_SelectedEntity)
			m_EntityPropertiePanel.OnImGuiRender(m_SelectedEntity);

		ImGui::End();

		ImGui::Begin("Scene");

		static int selection_mask = 0x02;
		int node_clicked = -1;

		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1);
		
		for (int i = 0; i < m_Entities.size(); i++)
		{
			ImGuiTreeNodeFlags flags = ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
			bool opened = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, m_Entities[i]->GetName());
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
				m_SelectedEntity = m_Entities[i];
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Object")) {
					m_Entities.erase(m_Entities.begin() + i);
					m_SelectedEntity = nullptr;
					node_clicked = -1;
				}
				ImGui::EndPopup();
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
		
		ImGui::PopStyleVar();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path file = path;
				AddGameObject(file.string());
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();


		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Open Resource Infos");
		ImGui::Text("Selected file: %s\nFile path: %s\nFile extension: %s\n", m_FileBrowser.GetInfos().m_SelectedFile.c_str(), m_FileBrowser.GetInfos().m_FilePath.c_str(), m_FileBrowser.GetInfos().m_FileExtension.c_str());
		ImGui::End();

		ImGui::End();
	}

	void Editor::OnEvent(Event& e)
	{
		m_Camera->OnEvent(e);
	}
	void Editor::AddGameObject(DEFAULT_OBJECT_TYPE type)
	{
		Entity* temp = new Entity();
		temp->SetId(m_Entities.size());
		temp->AddComponent<TransformComponent>();
		temp->AddComponent<MaterialComponent>("Assets/Textures/diffuse.png");
		temp->AddComponent<RenderComponent>();

		switch (type)
		{
		case CUBE:
			temp->SetName("Cube");
			temp->AddComponent<ModelComponent>("Assets/Models/cube.obj");
			break;
		case SPHERE:
			temp->SetName("Sphere");
			temp->AddComponent<ModelComponent>("Assets/Models/sphere.obj");
			break;
		case PLANE:
			temp->SetName("Plane");
			temp->AddComponent<ModelComponent>("Assets/Models/plane.obj");
			break;
		}

		m_Entities.push_back(temp);
	}

	void Editor::AddGameObject(const std::string& file)
	{
		const size_t slash = file.find_last_of("/\\");
		const std::string m_SelectedFile = file.substr(slash + 1);

		size_t lastindex = m_SelectedFile.find_last_of(".");
		const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

		m_Models.emplace(m_FileName, Renderer::CreateModel(file));

		Entity* temp = new Entity();
		temp->SetId(m_Entities.size());
		temp->SetName(m_FileName);
		temp->AddComponent<TransformComponent>();
		temp->AddComponent<ModelComponent>(file);
		temp->AddComponent<MaterialComponent>("Assets/Textures/diffuse.png");
		temp->AddComponent<RenderComponent>();
		m_Entities.push_back(temp);
	}

	void Editor::OpenExternalFile()
	{
		m_FileBrowser.OpenFile();

		std::filesystem::path sourceFile = m_FileBrowser.GetInfos().m_FilePath;
		std::filesystem::path targetParent = "Assets";
		auto target = targetParent / sourceFile.filename();

		try
		{
			std::filesystem::create_directories(targetParent);
			std::filesystem::copy_file(sourceFile, target, std::filesystem::copy_options::overwrite_existing);
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
		}
	}
}