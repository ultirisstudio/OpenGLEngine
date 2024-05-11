#include "Viewport.h"
#include "../../Utils/Macros.h"

#include <imgui.h>

#include <OpenGLEngine/Core/Application.h>

OpenGLEngine::Viewport::Viewport() : m_ViewportSize({ 0.0f, 0.0f})
{
	FramebufferSpecification spec;
	spec.Width = Application::Get().GetWindow().GetWidth();
	spec.Height = Application::Get().GetWindow().GetHeight();
	spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };

	m_ViewportFrameBuffer = Framebuffer::Create(spec);
}

void OpenGLEngine::Viewport::Render(Scene& scene)
{
	if (!scene.getActiveCamera())
		return;

	m_ViewportFrameBuffer->Bind();

	Renderer::SetViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

	Renderer::Clear();
	Renderer::ClearColor(glm::vec4(0.5f, 0.5f, .5f, 1.0f));

	Renderer::BeginScene(scene);
	Renderer::Render(*scene.getActiveCamera());
	Renderer::RenderSkybox(*scene.getActiveCamera());
	Renderer::EndScene();

	m_ViewportFrameBuffer->Unbind();
}

void OpenGLEngine::Viewport::OnImGuiRender(Scene& scene)
{
	m_ViewportFrameBuffer->Bind();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");

	if (scene.getActiveCamera())
	{
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImVec2 viewportPanelPos = ImGui::GetWindowPos();

		scene.getActiveCamera()->OnResize(viewportPanelSize.x, viewportPanelSize.y);

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_ViewportFrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		uint32_t textureID = m_ViewportFrameBuffer->GetColorAttachment(0);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	ImGui::End();
	ImGui::PopStyleVar();

	m_ViewportFrameBuffer->Unbind();
}
