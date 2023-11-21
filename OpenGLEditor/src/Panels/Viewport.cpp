#include "Viewport.h"

#include <imgui.h>

#include <OpenGLEngine/Core/Application.h>

OpenGLEngine::Viewport::Viewport() : m_ViewportSize({ 0.0f, 0.0f})
{
	m_ViewportFrameBuffer = std::make_shared<Framebuffer>(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	m_ViewportFrameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	m_ViewportFrameBuffer->setDepthAttachment();
	m_ViewportFrameBuffer->Create();
}

void OpenGLEngine::Viewport::Render(Scene& scene)
{
	if (!scene.getActiveCamera())
		return;

	m_ViewportFrameBuffer->bind();

	glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

	Renderer::Clear();
	Renderer::ClearColor(glm::vec4(0.5f, 0.5f, .5f, 1.0f));

	Renderer::BeginScene(scene);
	Renderer::Render(true);
	Renderer::EndScene();

	m_ViewportFrameBuffer->unbind();
}

void OpenGLEngine::Viewport::OnImGuiRender(Scene& scene)
{
	m_ViewportFrameBuffer->bind();

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
			m_ViewportFrameBuffer = std::make_shared<Framebuffer>((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportFrameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
			m_ViewportFrameBuffer->setDepthAttachment();
			m_ViewportFrameBuffer->Create();

			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		uint32_t textureID = m_ViewportFrameBuffer->getColorAttachment(0);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	ImGui::End();
	ImGui::PopStyleVar();

	m_ViewportFrameBuffer->unbind();
}
