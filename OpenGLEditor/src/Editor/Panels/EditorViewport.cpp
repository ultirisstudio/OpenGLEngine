#include "EditorViewport.h"
#include "../../Utils/Macros.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include "../Editor.h"

#include <OpenGLEngine/Core/Application.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Tools/Math.h>
#include <OpenGLEngine/Core/Input.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace OpenGLEngine
{
	EditorViewport::EditorViewport() : m_EditorViewportSize({ 0.0f, 0.0f })
	{
		FramebufferSpecification spec;
		spec.Width = Application::Get().GetWindow().GetWidth();
		spec.Height = Application::Get().GetWindow().GetHeight();
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

		m_EditorFrameBuffer = Framebuffer::Create(spec);
	}

	void EditorViewport::Render(Scene& scene, EditorCamera& camera)
	{
		m_EditorFrameBuffer->Bind();

		Renderer::Clear();
		Renderer::ClearColor(glm::vec4(0.5f, 0.5f, .5f, 1.0f));

		m_EditorFrameBuffer->ClearAttachment(1, -1);

		Renderer::BeginScene(scene);
		Renderer::Render(camera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_EditorViewportBounds[0].x;
		my -= m_EditorViewportBounds[0].y;
		glm::vec2 editorViewportSize = m_EditorViewportBounds[1] - m_EditorViewportBounds[0];
		my = editorViewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < editorViewportSize.x && mouseY < editorViewportSize.y)
		{
			int pixelData = m_EditorFrameBuffer->ReadPixel(1, mouseX, mouseY);
			
			std::cout << "Pixel data = " << pixelData << std::endl;
			//std::cout << ((pixelData < 0) ? "" : scene.getEntities()->at(pixelData).GetName()) << std::endl;

			m_HoveredEntity = scene.GetEntityByUUID(pixelData);
		}

		Renderer::RenderSkybox(camera);
		Renderer::EndScene();

		m_EditorFrameBuffer->Unbind();
	}

	void EditorViewport::Update(EditorCamera& camera)
	{
		if (m_ViewportHovered)
			camera.m_CameraFocus = true;
		else
			camera.m_CameraFocus = false;

		if (Input::IsKeyPressed(Key::E))
			m_GizmoType = -1;

		if (Input::IsKeyPressed(Key::R))
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

		if (Input::IsKeyPressed(Key::T))
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;

		if (Input::IsKeyPressed(Key::Y))
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}

	void EditorViewport::OnImGuiRender(EditorCamera& camera, SceneManager& sceneManager, SceneHierarchy& sceneHierarchy)
	{
		m_EditorFrameBuffer->Bind();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Editor");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		auto windowSize = ImGui::GetWindowSize();
		auto viewportOffset = ImGui::GetCursorPos();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();


		camera.OnResize(viewportPanelSize.x, viewportPanelSize.y);

		if (m_EditorViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_EditorFrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_EditorViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		uint32_t textureID = m_EditorFrameBuffer->GetColorAttachment(0);
		ImGui::Image((void*)textureID, ImVec2{ m_EditorViewportSize.x, m_EditorViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_EditorViewportBounds[0] = { minBound.x, minBound.y };
		m_EditorViewportBounds[1] = { maxBound.x, maxBound.y };

		//std::cout << "Min Bounds = " << m_EditorViewportBounds[0].x << ", " << m_EditorViewportBounds[0].y << std::endl;
		//std::cout << "Max Bounds = " << m_EditorViewportBounds[1].x << ", " << m_EditorViewportBounds[1].y << std::endl;

		/////////////////////////////////////////////////////////////////////////////////////////////

		if (sceneHierarchy.m_SelectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			glm::mat4 cameraProjection = camera.getProjectionMatrix();
			glm::mat4 cameraView = camera.getViewMatrix();

			auto& tc = sceneHierarchy.m_SelectedEntity->GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing())
			{
				if (ImGuizmo::IsOver())
					camera.useGuizmo();
				else
					camera.unuseGuizmo();

				glm::vec3 position, scale;
				glm::quat rotationQuat;
				glm::decompose(transform, scale, rotationQuat, position, glm::vec3(), glm::vec4());
				glm::vec3 rotation = glm::eulerAngles(rotationQuat); // * 3.14159f / 180.f

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Position = position;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
			else
				camera.unuseGuizmo();
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

				if (fileExtension == "obj")
					sceneManager.AddGameObject(filePath);

				if (fileExtension == "scene")
				{
					sceneHierarchy.m_SelectedEntity = nullptr;
					sceneManager.LoadScene(filePath);
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Editor infos");
		ImGui::Text(std::string(std::string("Hovered entity: ") + std::string(m_HoveredEntity ? m_HoveredEntity->GetName() : "none")).c_str());
		ImGui::End();

		m_EditorFrameBuffer->Unbind();
	}
}