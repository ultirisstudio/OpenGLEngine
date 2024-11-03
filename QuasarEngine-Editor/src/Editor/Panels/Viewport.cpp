#include "Viewport.h"

#include <imgui.h>

#include <QuasarEngine/Core/Application.h>

QuasarEngine::Viewport::Viewport() : m_ViewportSize({ 0.0f, 0.0f})
{
	FramebufferSpecification spec;
	spec.Width = Application::Get().GetWindow().GetWidth();
	spec.Height = Application::Get().GetWindow().GetHeight();
	spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };

	m_ViewportFrameBuffer = Framebuffer::Create(spec);
}

void QuasarEngine::Viewport::Render(SceneObject& sceneObject)
{
	if (!sceneObject.HasPrimaryCamera())
		return;

	m_ViewportFrameBuffer->Bind();

	RenderCommand::SetViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

	RenderCommand::Clear();
	RenderCommand::ClearColor(glm::vec4(0.5f, 0.5f, .5f, 1.0f));

	Renderer::BeginScene(sceneObject.GetScene());
	Renderer::Render(sceneObject.GetPrimaryCamera());
	Renderer::RenderSkybox(sceneObject.GetPrimaryCamera());
	Renderer::EndScene();

	m_ViewportFrameBuffer->Unbind();
}

void QuasarEngine::Viewport::OnImGuiRender(SceneObject& sceneObject)
{
	m_ViewportFrameBuffer->Bind();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");

	if (sceneObject.HasPrimaryCamera())
	{
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImVec2 viewportPanelPos = ImGui::GetWindowPos();

		sceneObject.GetPrimaryCamera().OnResize(viewportPanelSize.x, viewportPanelSize.y);
		RenderCommand::SetViewport(0, 0, viewportPanelSize.x, viewportPanelSize.y);

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
