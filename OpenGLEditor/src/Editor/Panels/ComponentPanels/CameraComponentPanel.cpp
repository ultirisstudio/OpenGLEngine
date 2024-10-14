#include "CameraComponentPanel.h"

#include <imgui.h>
#include <OpenGLEngine/ECS/Entity.h>
#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/ECS/Components/CameraComponent.h>
#include <OpenGLEngine/Scene/Camera.h>

namespace OpenGLEngine
{
	void CameraComponentPanel::Render(Entity entity, SceneManager& sceneManager)
	{
		if (entity.HasComponent<CameraComponent>())
		{
			auto& cc = entity.GetComponent<CameraComponent>();

			if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				ImGui::Text("Camera parameters: ");

				ImGui::Separator();

				if (cc.Primary) ImGui::Text("Is Primary");
				if (ImGui::Button("Set to primary"))
				{
					/*for (auto e : sceneManager.GetActiveScene().GetAllEntitiesWith<CameraComponent>())
					{
						Entity primaryCameraEntity = sceneManager.GetSceneObject().GetPrimaryCameraEntity();
						if (&primaryCameraEntity.GetComponent<CameraComponent>().GetCamera() != &cc.GetCamera())
						{
							if (primaryCameraEntity.GetComponent<CameraComponent>().Primary == true)
							{
								primaryCameraEntity.GetComponent<CameraComponent>().Primary = false;
								cc.Primary = true;
							}
						}
					}*/
				}

				ImGui::Separator();

				ImGui::Text("FOV: "); ImGui::SameLine();
				float fov = cc.GetCamera().GetFov();
				if (ImGui::DragFloat("##FOV", &fov, 0.1f, 0.0f, 180.0f, "%.1f"))
				{
					cc.GetCamera().SetFov(fov);
				}

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<CameraComponent>();
					}
					ImGui::EndPopup();
				}
				ImGui::TreePop();
			}
		}
	}
}