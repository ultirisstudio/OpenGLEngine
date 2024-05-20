#include "Runtime.h"

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

namespace OpenGLEngine
{
	Runtime::Runtime() : Layer("Editor"), m_Chronometer(false)
	{
		//Application::Get().MaximizeWindow(true);
	}

	void Runtime::OnAttach()
	{
		ScriptEngine::Init();
		PhysicEngine::Init();
		PerlinManager::Init();
		Renderer::Init();

		Renderer::SetViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

		ScriptEngine::SetAssemblyPath(std::filesystem::current_path().generic_string() + "\\Scripts\\OpenGLEngine-ScriptCore.dll");
		ScriptEngine::SetAppAssemblyPath(std::filesystem::current_path().generic_string() + "\\Assets\\Scripts\\CallOf.dll");
		ScriptEngine::ReloadAssembly();

		m_ScreenQuad = std::make_unique<ScreenQuad>();

		m_Shader = std::make_unique<Shader>();
		m_Shader->LoadFromFile("Shaders/ScreenQuad.vert", "Shaders/ScreenQuad.frag");
		m_Shader->setUniform("screenTexture", 0);

		FramebufferSpecification spec;
		spec.Width = Application::Get().GetWindow().GetWidth();
		spec.Height = Application::Get().GetWindow().GetHeight();
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };

		m_FrameBuffer = Framebuffer::Create(spec);

		m_SceneManager = std::make_unique<SceneManager>();
		m_SceneManager->LoadScene(std::filesystem::current_path().generic_string() + "\\Assets\\Scenes\\save_test.scene");
		uint32_t width = Application::Get().GetWindow().GetWidth();
		uint32_t height = Application::Get().GetWindow().GetHeight();
		m_SceneManager->getActiveScene().getActiveCamera()->OnResize(width, height);
		Renderer::SetViewport(0, 0, width, height);
	}

	void Runtime::OnDetach()
	{
		ScriptEngine::Shutdown();
		PhysicEngine::Shutdown();
	}

	void Runtime::OnUpdate(double dt)
	{
		//m_Chronometer.restart();

		if (Input::IsKeyPressed(Key::Space))
			m_SceneManager->getActiveScene().OnRuntimeStart();

		CalculateLatency();
		Application::Get().GetWindow().SetTitle("Runtime [" + std::to_string(fps) + ":" + std::to_string(latency) + "]");

		m_SceneManager->update(dt);

		if (!m_SceneManager->getActiveScene().getActiveCamera())
			return;

		m_FrameBuffer->Bind();
		Renderer::Clear();
		Renderer::ClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		Renderer::BeginScene(m_SceneManager->getActiveScene());

		Renderer::Render(*m_SceneManager->getActiveScene().getActiveCamera());

		Renderer::RenderSkybox(*m_SceneManager->getActiveScene().getActiveCamera());

		Renderer::EndScene();
		m_FrameBuffer->Unbind();
		m_Shader->use();
		m_FrameBuffer->BindColorAttachment();
		m_ScreenQuad->Draw();

		//std::cout << "Total Render: " << m_Chronometer2.getElapsedTime().milliseconds << "ms" << std::endl;
		//m_Chronometer.stop();
	}

	void Runtime::CalculateLatency()
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