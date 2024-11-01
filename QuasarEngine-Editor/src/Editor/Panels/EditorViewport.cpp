#include "EditorViewport.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include "../Editor.h"

#include <QuasarEngine/Core/Application.h>
#include <QuasarEngine/Entity/Components/TransformComponent.h>
#include <QuasarEngine/Entity/Components/HierarchyComponent.h>
#include <QuasarEngine/Tools/Math.h>
#include <QuasarEngine/Core/Input.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace QuasarEngine
{
	EditorViewport::EditorViewport() : m_EditorViewportSize({ 0.0f, 0.0f })
	{
		FramebufferSpecification spec;
		spec.Width = Application::Get().GetWindow().GetWidth();
		spec.Height = Application::Get().GetWindow().GetHeight();
		//spec.Samples = 4;
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

		//auto [mx, my] = ImGui::GetMousePos();
		//mx -= m_EditorViewportBounds[0].x;
		//my -= m_EditorViewportBounds[0].y;
		glm::vec2 editorViewportSize = m_EditorViewportBounds[1] - m_EditorViewportBounds[0];
		//my = editorViewportSize.y - my;
		//int mouseX = (int)mx;
		//int mouseY = (int)my - m_WindowTitleBarSize[1];

		//if (mouseX >= 0 && mouseY >= 0 && mouseX < editorViewportSize.x && mouseY < editorViewportSize.y)
		//{
		//	int pixelData = m_EditorFrameBuffer->ReadPixel(1, mouseX, mouseY);
//
		//	m_HoveredEntity = scene.GetEntityByUUID(pixelData);
		//}

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

		m_WindowTitleBarSize = { ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMin().y };

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

			auto& tc = sceneHierarchy.m_SelectedEntity.GetComponent<TransformComponent>();

			glm::mat4 transform = tc.GetGlobalTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing())
			{
				if (ImGuizmo::IsOver())
				{
					camera.useGuizmo();
				}
				else
				{
					camera.unuseGuizmo();
				}

				glm::mat4 finalTransform = transform;

				UUID parentID = sceneHierarchy.m_SelectedEntity.GetComponent<HierarchyComponent>().m_Parent;
				while (parentID != UUID::Null())
				{
					Entity parent = Renderer::m_SceneData.m_Scene->GetEntityByUUID(parentID);
					if (parent)
					{
						glm::mat4 parentTransform = parent.GetComponent<TransformComponent>().GetLocalTransform();

						finalTransform *= glm::inverse(parentTransform);

						parentID = parent.GetComponent<HierarchyComponent>().m_Parent;
					}
				}

				glm::vec3 position, scale;
				glm::quat rotationQuat;
				glm::decompose(finalTransform, scale, rotationQuat, position, glm::vec3(), glm::vec4());
				//glm::vec3 rotation = glm::eulerAngles(rotationQuat);
				//glm::vec3 deltaRotation = rotation - tc.Rotation;

				tc.Position = position;
				//tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
			else
			{
				camera.unuseGuizmo();
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

				if (fileExtension == "obj" || fileExtension == "dae" || fileExtension == "fbx" || fileExtension == "glb" || fileExtension == "gltf")
					sceneManager.AddGameObject(filePath);

				if (fileExtension == "scene")
				{
					//sceneHierarchy.m_SelectedEntity = entt::null;
					sceneManager.LoadScene(filePath);
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Editor infos");
		ImGui::Text(std::string(std::string("Hovered entity: ") + std::string(m_HoveredEntity ? m_HoveredEntity.GetName() : "none")).c_str());
		ImGui::End();

		m_EditorFrameBuffer->Unbind();
	}
}