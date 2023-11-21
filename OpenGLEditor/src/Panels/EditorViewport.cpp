#include "EditorViewport.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <OpenGLEngine/Core/Application.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Tools/Math.h>
#include <OpenGLEngine/Core/Input.h>

#include <glm/gtc/type_ptr.hpp>


namespace OpenGLEngine
{
	EditorViewport::EditorViewport() : m_EditorViewportSize({ 0.0f, 0.0f })
	{
		m_EditorFrameBuffer = std::make_shared<Framebuffer>(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		m_EditorFrameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		m_EditorFrameBuffer->setDepthAttachment();
		m_EditorFrameBuffer->Create();
	}

	void EditorViewport::Render(Scene& scene)
	{
		m_EditorFrameBuffer->bind();

		Renderer::Clear();
		Renderer::ClearColor(glm::vec4(0.5f, 0.5f, .5f, 1.0f));

		Renderer::BeginScene(scene);
		Renderer::Render(false);
		Renderer::EndScene();

		m_EditorFrameBuffer->unbind();
	}

	void EditorViewport::Update()
	{
		if (Input::IsKeyPressed(Key::E))
			m_GizmoType = -1;

		if (Input::IsKeyPressed(Key::R))
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

		if (Input::IsKeyPressed(Key::T))
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;

		if (Input::IsKeyPressed(Key::Y))
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}

	void EditorViewport::OnImGuiRender(Scene& scene)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Editor");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImVec2 viewportPanelPos = ImGui::GetWindowPos();
		int x, y;
		glfwGetWindowPos(reinterpret_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), &x, &y);

		scene.ResizeEditorCamera(viewportPanelSize.x, viewportPanelSize.y);

		if (m_EditorViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_EditorFrameBuffer = std::make_shared<Framebuffer>((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_EditorFrameBuffer->addColorAttachment(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
			m_EditorFrameBuffer->setDepthAttachment();
			m_EditorFrameBuffer->Create();

			m_EditorViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			//m_Scene->ResizeCamera(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = m_EditorFrameBuffer->getColorAttachment(0);
		ImGui::Image((void*)textureID, ImVec2{ m_EditorViewportSize.x, m_EditorViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		/////////////////////////////////////////////////////////////////////////////////////////////

		if (scene.m_SelectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			glm::mat4 cameraProjection = scene.getEditorCamera().getProjectionMatrix();
			glm::mat4 cameraView = scene.getEditorCamera().getViewMatrix();

			auto& tc = scene.m_SelectedEntity->GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing)
			{
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transform, position, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Position = position;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				//std::filesystem::path file = path;
				std::wstring ws(path);
				std::string filePath(ws.begin(), ws.end());
				const size_t slash = filePath.find_last_of("/\\");
				std::string selectedFile = filePath.substr(slash + 1);
				std::string fileExtension = selectedFile.substr(selectedFile.find_last_of(".") + 1);

				//if (fileExtension == "obj")
					//AddGameObject(filePath);

				if (fileExtension == "scene")
				{
					//LoadScene(filePath);
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();

		ImGui::PopStyleVar();
	}
}