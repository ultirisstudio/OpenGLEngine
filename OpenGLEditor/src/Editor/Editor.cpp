#include "Editor.h"

#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"

#include "OpenGLEngine/Core/MouseCodes.h"
#include "OpenGLEngine/Scripting/ScriptEngine.h"
#include "OpenGLEngine/Physic/PhysicEngine.h"

#include "OpenGLEngine/Perlin/PerlinManager.h"

#include "mbedtls/aes.h"

#include <yaml-cpp/yaml.h>

#include "Export.h"

namespace OpenGLEngine
{
	std::vector<unsigned char> convert_to_unsigned_char(const std::vector<char>& original) {
		std::vector<unsigned char> converted(original.size());
		std::transform(original.begin(), original.end(), converted.begin(),
			[](char c) { return static_cast<unsigned char>(c); });
		return converted;
	}

	Editor::Editor(const EditorSpecification& spec)
		: Layer("Editor"), m_Specification(spec),
		m_EntityPropertiePanel(),
		m_SceneHierarchy(),
		m_ContentBrowserPanel(spec.ProjectPath),
		m_Viewport(),
		m_EditorViewport(),
		m_Chronometer(false),
		m_EditorCamera(std::make_unique<EditorCamera>(glm::vec3(0.0f, 0.0f, 6.0f)))
	{
		Application::Get().MaximizeWindow(true);
	}

	void Editor::OnAttach()
	{
		InitImGuiStyle();

		ScriptEngine::Init();
		PhysicEngine::Init();
		PerlinManager::Init();
		Renderer::Init();

		m_SceneManager = std::make_unique<SceneManager>();
		m_SceneManager->LoadScene(m_Specification.ProjectPath + "\\Assets\\c.scene");

		ScriptEngine::SetAssemblyPath(std::filesystem::current_path().generic_string() + "\\Scripts\\OpenGLEngine-ScriptCore.dll");
		ScriptEngine::SetAppAssemblyPath(m_Specification.ProjectPath + "\\Scripts\\Build\\" + m_Specification.ProjectName + ".dll");

		ScriptEngine::ReloadAssembly();

		//////////////////////////////////////////////

		/*mbedtls_aes_context aes;
		mbedtls_aes_context aes2;

		unsigned char key[16] = "itzkbgulrcsjmnv";

		unsigned char eiv[16] = { 0xb2, 0x4b, 0xf2, 0xf7, 0x7a, 0xc5, 0xec, 0x0c, 0x5e, 0x1f, 0x4d, 0xc1, 0xae, 0x46, 0x5e, 0x75 };
		unsigned char div[16] = { 0xb2, 0x4b, 0xf2, 0xf7, 0x7a, 0xc5, 0xec, 0x0c, 0x5e, 0x1f, 0x4d, 0xc1, 0xae, 0x46, 0x5e, 0x75 };

		unsigned char input[128] = "Julien";
		unsigned char output[128];
		unsigned char output2[128];

		size_t input_len = 40;
		size_t output_len = 0;

		mbedtls_aes_setkey_enc(&aes, key, 256);
		mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, 48, eiv, input, output);

		std::cout << output << std::endl;

		mbedtls_aes_setkey_dec(&aes2, key, 256);
		mbedtls_aes_crypt_cbc(&aes2, MBEDTLS_AES_DECRYPT, 48, div, output, output2);

		std::cout << output2 << std::endl;*/

		/////////////////////////////////////////

		/*std::vector<std::filesystem::path> ressources;
		ressources.push_back("Assets\\Textures\\diffuse.png");
		//ressources.push_back("Assets\\Textures\\specular.png");
		//ressources.push_back("Assets\\Textures\\OTskLEus.jpg");
		//ressources.push_back("Assets\\Textures\\YgK4ozkE.png");
		//ressources.push_back("Assets\\Models\\BackPack.obj");
		Export::CreatePakFile(ressources, "Test/ressources.pak", true);

		std::unordered_map<std::string, std::vector<char>> loaded_ressources = Export::LoadAllResourcesFromPak("Test/ressources.pak");

		for (auto& [key, value] : loaded_ressources) {
			std::cout << key << std::endl;

			std::vector<unsigned char> uData = convert_to_unsigned_char(value);

			m_TextureTest = Texture::CreateTexture(uData, false);
		}*/
	}

	void Editor::OnDetach()
	{
		ScriptEngine::Shutdown();
		PhysicEngine::Shutdown();
	}

	void Editor::OnUpdate(double dt)
	{
		CalculateLatency();
		Application::Get().GetWindow().SetTitle("OpenGLEditor [" + std::to_string(fps) + ":" + std::to_string(latency) + "]");

		m_EditorCamera->Update();

		m_SceneManager->update(dt);

		m_EditorViewport.Render(m_SceneManager->getActiveScene(), *m_EditorCamera);
		m_EditorViewport.Update(*m_EditorCamera);

		m_Viewport.Render(m_SceneManager->getActiveScene());

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
					m_SceneManager->ReloadScene(m_SceneManager->getActiveScene().getPath());

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Create", hasProject))
			{
				if (ImGui::MenuItem("Create new GameObject"))
				{
					Entity* temp = m_SceneManager->getActiveScene().CreateEntity("GameObject");
				}

				if (ImGui::MenuItem("Create new Cube")) m_SceneManager->AddCube();
				if (ImGui::MenuItem("Create new Sphere")) m_SceneManager->AddSphere();
				if (ImGui::MenuItem("Create new UV Sphere")) m_SceneManager->AddUVSphere();
				if (ImGui::MenuItem("Create new Plane")) m_SceneManager->AddPlane();;
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
		m_EditorViewport.OnImGuiRender(*m_EditorCamera, *m_SceneManager, m_SceneHierarchy);
		m_EntityPropertiePanel.OnImGuiRender(*m_SceneManager, m_SceneHierarchy);
		m_SceneHierarchy.OnImGuiRender(m_SceneManager->getActiveScene());
		m_ContentBrowserPanel.OnImGuiRender();

		/*ImGui::Begin("World infos:");
		{
			
		}
		ImGui::End();*/

		/*ImGui::Begin("Test");
		{
			if (m_TextureTest)
				ImGui::ImageButton((ImTextureID)m_TextureTest->GetID(), { 64, 64 }, { 0, 1 }, { 1, 0 });
		}
		ImGui::End();*/

		if (m_optionMenu)
		{
			OptionMenu();
		}

		ImGui::End();
	}

	void Editor::OnEvent(Event& e)
	{
		m_EditorCamera->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Editor::OnMouseButtonPressed, this, std::placeholders::_1));
	}

	bool Editor::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (ImGuizmo::IsOver())
			return false;

		if (!m_EditorViewport.IsViewportHovered())
			return false;

		if (e.GetMouseButton() == Mouse::Button0)
		{
			m_SceneHierarchy.m_SelectedEntity = m_EditorViewport.GetHoveredEntity();
			return true;
		}

		return false;
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
			"Border"
		};

		std::ifstream fin(std::filesystem::current_path().generic_string() + "/theme.ini");
		if (fin.is_open())
		{
			YAML::Node data = YAML::Load(fin);
			for (int temp = 0; temp < m_ImGuiColor.size(); temp++)
			{
				m_ThemeColor[m_ImGuiColor[temp]] = { data[m_ThemeName[temp]]["x"].as<float>(), data[m_ThemeName[temp]]["y"].as<float>(), data[m_ThemeName[temp]]["z"].as<float>(), data[m_ThemeName[temp]]["w"].as<float>() };
				ImGuiStyle* style = &ImGui::GetStyle();
				ImVec4* colors = style->Colors;
				colors[m_ImGuiColor[temp]] = ImVec4(m_ThemeColor[m_ImGuiColor[temp]].x, m_ThemeColor[m_ImGuiColor[temp]].y, m_ThemeColor[m_ImGuiColor[temp]].z, m_ThemeColor[m_ImGuiColor[temp]].w);
			}
			fin.close();
		}
	}

	void Editor::OptionMenu()
	{
		ImGui::Begin("Preference");
		{
			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, 230);

			if (ImGui::Button("Theme", ImVec2(230 - 15, 39)))
				m_optionTab = 0;

			if (ImGui::Button("Save", ImVec2(230 - 15, 39)))
			{
				YAML::Emitter out;
				out << YAML::BeginMap;
				for (int temp = 0; temp < m_ImGuiColor.size(); temp++)
				{
					out << m_ThemeName[temp] << YAML::BeginMap;
					out << "x" << m_ThemeColor[m_ImGuiColor[temp]].x;
					out << "y" << m_ThemeColor[m_ImGuiColor[temp]].y;
					out << "z" << m_ThemeColor[m_ImGuiColor[temp]].z;
					out << "w" << m_ThemeColor[m_ImGuiColor[temp]].w;
					out << YAML::EndMap;
				}
				out << YAML::EndMap;
				std::ofstream fout(std::filesystem::current_path().generic_string() + "/theme.ini");
				fout << out.c_str();
				fout.close();
			}

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
			ImVec4* colors = style->Colors;

			for (int temp1 = 0; temp1 < m_ImGuiColor.size(); temp1++)
			{
				m_ThemeColor[m_ImGuiColor[temp1]] = { colors[m_ImGuiColor[temp1]].x, colors[m_ImGuiColor[temp1]].y, colors[m_ImGuiColor[temp1]].z, colors[m_ImGuiColor[temp1]].w };
			}

			for (int temp2 = 0; temp2 < m_ImGuiColor.size(); temp2++)
			{
				std::string temp("##" + std::string(m_ThemeName[temp2]));
				ImGui::Text(m_ThemeName[temp2]); ImGui::SameLine(); ImGui::ColorEdit4(temp.c_str(), glm::value_ptr(m_ThemeColor[m_ImGuiColor[temp2]]));
			}

			for (int temp3 = 0; temp3 < m_ImGuiColor.size(); temp3++)
			{
				colors[m_ImGuiColor[temp3]] = ImVec4(m_ThemeColor[m_ImGuiColor[temp3]].x, m_ThemeColor[m_ImGuiColor[temp3]].y, m_ThemeColor[m_ImGuiColor[temp3]].z, m_ThemeColor[m_ImGuiColor[temp3]].w);
			}
		}

		ImGui::End();
	}

	void Editor::CalculateLatency()
	{
		double currentTime = Renderer::GetTime();
		nb_frame++;
		if (currentTime - last_time >= 1.0) {
			latency = (1000.0 / double(nb_frame));
			fps = nb_frame;
			nb_frame = 0;
			last_time += 1.0;
		}
	}
}
