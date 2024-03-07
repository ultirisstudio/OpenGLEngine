#include "Editor.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "imgui.h"
#include "ImGuizmo.h"

#include "OpenGLEngine/Scripting/ScriptEngine.h"
#include "OpenGLEngine/Perlin/PerlinManager.h"

namespace OpenGLEngine
{
	Editor::Editor(const EditorSpecification& spec) : Layer("Editor"), m_Specification(spec), m_ContentBrowserPanel(spec.ProjectPath), m_EntityPropertiePanel(), m_SceneHierarchy(), m_Viewport(), m_EditorViewport(), m_Chronometer(false)
	{
		Application::Get().MaximizeWindow(true);

		//std::cout << spec.EngineExecutablePath << std::endl;
		//std::cout << spec.ProjectName << std::endl;
		//std::cout << spec.ProjectPath << std::endl;
	}

	void Editor::OnAttach()
	{
		InitImGuiStyle();

		ScriptEngine::Init();
		PerlinManager::Init();
		Renderer::Init();

		m_SceneManager = std::make_unique<SceneManager>();
		m_SceneManager->LoadScene(m_Specification.ProjectPath + "\\Assets\\test.scene");

		ScriptEngine::SetAppAssemblyPath(m_Specification.ProjectPath + "\\Scripts\\Build\\Intermediates\\Release\\" + m_Specification.ProjectName + ".dll");
		ScriptEngine::ReloadAssembly();

		//m_SceneManager->getActiveScene().OnRuntimeStart();
	}

	void Editor::OnDetach()
	{
		ScriptEngine::Shutdown();
	}

	void Editor::OnUpdate(double dt)
	{
		m_SceneManager->update(dt);
		m_EditorViewport.Update(m_SceneManager->getActiveScene());

		m_Viewport.Render(m_SceneManager->getActiveScene());
		m_EditorViewport.Render(m_SceneManager->getActiveScene());

		if (Input::IsKeyPressed(Key::LeftControl))
		{
			if (Input::IsKeyPressed(Key::S))
			{
				m_SceneManager->SaveScene();
			}
		}
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

		bool hasProject = true;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("Quit"))
					OpenGLEngine::Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene", hasProject))
			{
				if (ImGui::MenuItem("New scene"))
				{
					m_SceneManager->createNewScene();
				}

				if (ImGui::MenuItem("Save scene"))
				{
					m_SceneManager->SaveScene();
				}

				if (ImGui::MenuItem("Load scene"))
				{
					m_SceneManager->LoadScene();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script", hasProject))
			{
				if (ImGui::MenuItem("Build"))
				{
					ScriptEngine::ReloadAssembly();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Runtime", hasProject))
			{
				if (ImGui::MenuItem("Start scene"))
				{
					m_SceneManager->SaveScene();
					m_SceneManager->getActiveScene().OnRuntimeStart();
				}
				if (ImGui::MenuItem("Stop scene"))
				{
					m_SceneManager->getActiveScene().OnRuntimeStop();
					m_SceneManager->LoadScene(m_SceneManager->getActiveScene().getPath());
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Create", hasProject))
			{
				if (ImGui::MenuItem("Create new GameObject"))
				{
					m_SceneManager->getActiveScene().CreateEntity("GameObject");
				}

				if (ImGui::MenuItem("Create new Cube")) m_SceneManager->AddGameObject(CUBE);
				if (ImGui::MenuItem("Create new Sphere")) m_SceneManager->AddGameObject(SPHERE);
				if (ImGui::MenuItem("Create new Plane")) m_SceneManager->AddGameObject(PLANE);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Preference"))
				{
					m_optionMenu = true;
				}
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				//ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_Viewport.OnImGuiRender(m_SceneManager->getActiveScene());
		m_EditorViewport.OnImGuiRender(*m_SceneManager);
		m_EntityPropertiePanel.OnImGuiRender(*m_SceneManager);
		m_SceneHierarchy.OnImGuiRender(m_SceneManager->getActiveScene());
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("World infos:");
		{
			ImGui::SliderFloat("Ambiant light", &m_SceneManager->getActiveScene().m_AmbientLight, 0.0f, 1.0f);
		}

		ImGui::End();

		if (m_optionMenu)
		{
			OptionMenu();
		}

		ImGui::End();
	}

	void Editor::OnEvent(Event& e)
	{
		m_SceneManager->getActiveScene().getEditorCamera().OnEvent(e);
	}

	void Editor::InitImGuiStyle()
	{
		m_ImGuiColor = {
			ImGuiCol_WindowBg,
			ImGuiCol_Header,
			ImGuiCol_HeaderHovered,
			ImGuiCol_HeaderActive,
			ImGuiCol_Button,
			ImGuiCol_ButtonHovered,
			ImGuiCol_ButtonActive,
			ImGuiCol_FrameBg,
			ImGuiCol_FrameBgHovered,
			ImGuiCol_FrameBgActive,
			ImGuiCol_Tab,
			ImGuiCol_TabHovered,
			ImGuiCol_TabActive,
			ImGuiCol_TabUnfocused,
			ImGuiCol_TabUnfocusedActive,
			ImGuiCol_TitleBg,
			ImGuiCol_TitleBgActive,
			ImGuiCol_TitleBgCollapsed,
			ImGuiCol_Border
		};

		m_ThemeName = {
			"WindowBg",
			"Header",
			"HeaderHovered",
			"HeaderActive",
			"Button",
			"ButtonHovered",
			"ButtonActive",
			"FrameBg",
			"FrameBgHovered",
			"FrameBgActive",
			"Tab",
			"TabHovered",
			"TabActive",
			"TabUnfocused",
			"TabUnfocusedActive",
			"TitleBg",
			"TitleBgActive",
			"TitleBgCollapsed",
			"ImGuiCol_Border"
		};
	}

	void Editor::OptionMenu()
	{
		ImGui::Begin("Preference");
		{
			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, 230);

			if (ImGui::Button("Theme", ImVec2(230 - 15, 39)))
				m_optionTab = 0;

			if (ImGui::Button("Close", ImVec2(230 - 15, 39)))
				m_optionMenu = false;
		}

		ImGui::NextColumn();

		{
			if (ImGui::Button("Real Dark", ImVec2(230 - 70, 29)))
				ImGui::StyleColorsRealDark();
			ImGui::SameLine();
			if (ImGui::Button("Dark", ImVec2(230 - 70, 29)))
				ImGui::StyleColorsDark();
			ImGui::SameLine();
			if (ImGui::Button("Classic", ImVec2(230 - 70, 29)))
				ImGui::StyleColorsClassic();
			ImGui::SameLine();
			if (ImGui::Button("Light", ImVec2(230 - 70, 29)))
				ImGui::StyleColorsLight();

			ImGuiStyle* style = &ImGui::GetStyle();

			for (int color : m_ImGuiColor)
			{
				m_ThemeColor[color] = { style->Colors[color].x, style->Colors[color].y, style->Colors[color].z, style->Colors[color].w };
			}

			for (int i = 0; i < m_ImGuiColor.size(); i++)
			{
				std::string temp("##" + std::string(m_ThemeName[i]));
				ImGui::Text(m_ThemeName[i]); ImGui::SameLine(); ImGui::ColorEdit4(temp.c_str(), glm::value_ptr(m_ThemeColor[i]));
			}

			for (int color : m_ImGuiColor)
			{
				style->Colors[color] = ImColor(m_ThemeColor[color].x, m_ThemeColor[color].y, m_ThemeColor[color].z, m_ThemeColor[color].w);
			}
		}

		ImGui::End();
	}
}