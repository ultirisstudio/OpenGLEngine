#include "Runtime.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "ImGuizmo.h"

#include <QuasarEngine/Physic/PhysicEngine.h>
#include <QuasarEngine/Entity/Components/CameraComponent.h>
#include <QuasarEngine/Entity/Components/LightComponent.h>
#include <QuasarEngine/Shader/Shader.h>

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Tools/Chronometer.h>

namespace QuasarEngine
{
	Runtime::Runtime() : Layer("Runtime")
	{
		//Application::Get().MaximizeWindow(true);
	}

	void Runtime::OnAttach()
	{
		RenderCommand::Init();

		/*PhysicEngine::Init();
		Renderer::Init();

		Application::Get().GetWindow().SetCursorVisibility(true);

		m_ScreenQuad = std::make_unique<ScreenQuad>();
		m_Scene = std::make_unique<Scene>();

		Renderer::BeginScene(*m_Scene);

		ShaderFile shaderfile;
		shaderfile.vertexShaderFile = "Assets/Shaders/ScreenQuad.vert";
		shaderfile.fragmentShaderFile = "Assets/Shaders/ScreenQuad.frag";
		m_ScreenQuadShader = Shader::Create(shaderfile);
		m_ScreenQuadShader->setUniform("screenTexture", 0);

		FramebufferSpecification spec;
		spec.Width = Application::Get().GetWindow().GetWidth();
		spec.Height = Application::Get().GetWindow().GetHeight();
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };

		m_FrameBuffer = Framebuffer::Create(spec);
		m_ApplicationSize = { spec.Width, spec.Height };
		RenderCommand::SetViewport(0, 0, m_ApplicationSize.x, m_ApplicationSize.y);
		m_FrameBuffer->Resize((uint32_t)m_ApplicationSize.x, (uint32_t)m_ApplicationSize.y);

		m_ChunkManager = std::make_unique<ChunkManager>();
		m_Player = std::make_unique<Player>();

		m_Player->GetCamera().OnResize(m_ApplicationSize.x, m_ApplicationSize.y);

		std::cout << "Voxel: " << sizeof(Block) << std::endl;
		std::cout << "Chunk: " << sizeof(Chunk) << std::endl;
		std::cout << "ChunkManager: " << sizeof(ChunkManager) << std::endl;
		std::cout << "Mesh: " << sizeof(Mesh) << std::endl;
		std::cout << "VoxelType: " << sizeof(BlockType) << std::endl;

		Entity light = m_Scene->CreateEntity("Light");
		light.GetComponent<TransformComponent>().Rotation = { 20, 90, 45};
		auto& light_component = light.AddComponent<LightComponent>();
		light_component.SetType(QuasarEngine::LightComponent::LightType::DIRECTIONAL);
		light_component.dir_power = 30.0f;*/

		/*Entity player_light = m_Scene->CreateEntity("PlayerLight");
		auto& player_light_component = player_light.AddComponent<LightComponent>();
		player_light_component.SetType(QuasarEngine::LightComponent::LightType::POINT);
		player_light_component.point_power = 60.0f;
		player_light_component.point_attenuation = 0.2f;*/
	}

	void Runtime::OnDetach()
	{
		//PhysicEngine::Shutdown();
	}

	void Runtime::OnUpdate(double dt)
	{
		/*m_Player->Update(dt);

		m_Player->GetCamera().Update();

		m_ChunkManager->UpdateChunk(m_Player->GetPosition(), dt);*/
	}

	void Runtime::OnRender()
	{
		//m_FrameBuffer->Bind();

		RenderCommand::Clear();
		RenderCommand::ClearColor(glm::vec4(0.1f, 0.5f, .9f, 1.0f));

		//RenderCommand::SetViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

		/*Renderer::BeginScene(*m_Scene);
		Renderer::Render(m_Player->GetCamera());
		Renderer::RenderSkybox(m_Player->GetCamera());
		Renderer::EndScene();

		unsigned int width = Application::Get().GetWindow().GetWidth();
		unsigned int height = Application::Get().GetWindow().GetHeight();

		if (m_ApplicationSize.x != width || m_ApplicationSize.y != height)
		{
			RenderCommand::SetViewport(0, 0, width, height);
			m_Player->GetCamera().OnResize(width, height);
			m_FrameBuffer->Resize((uint32_t)width, (uint32_t)height);

			m_ApplicationSize = { width, height };
		}

		m_FrameBuffer->Unbind();

		m_ScreenQuadShader->use();
		m_FrameBuffer->BindColorAttachment(0);
		m_ScreenQuad->Draw();*/
	}

	void Runtime::OnGuiRender()
	{
		/*static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
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
					QuasarEngine::Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Asset"))
			{
				if (ImGui::MenuItem("Import"))
				{
					
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene", hasProject))
			{
				if (ImGui::MenuItem("New scene"))
				{
					
				}

				if (ImGui::MenuItem("Save scene"))
				{
					
				}

				if (ImGui::MenuItem("Load scene"))
				{
					
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script", hasProject))
			{
				if (ImGui::MenuItem("Build"))
				{
					
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Runtime", hasProject))
			{
				if (ImGui::MenuItem("Start scene"))
				{
					
				}
				if (ImGui::MenuItem("Stop scene"))
				{
					

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Create", hasProject))
			{
				if (ImGui::MenuItem("Create new GameObject"))
				{
					
				}

				if (ImGui::MenuItem("Create new Cube")) {};
				if (ImGui::MenuItem("Create new Sphere")) {};
				if (ImGui::MenuItem("Create new UV Sphere")) {};
				if (ImGui::MenuItem("Create new Plane")) {};
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Preference"))
				{
					
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Test");
		{
			
		}
		ImGui::End();

		ImGui::End();*/

		ImGui::Begin("Test");
		{

		}
		ImGui::End();
	}

	void Runtime::OnEvent(Event& e)
	{
		//m_Player->GetCamera().OnEvent(e);
	}
}
