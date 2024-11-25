#include "CameraComponentPanel.h"

#include <imgui.h>
#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/CameraComponent.h>
#include <QuasarEngine/Scene/Camera.h>

namespace QuasarEngine
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
				else
				{
					if (ImGui::Button("Set to primary"))
					{
						Entity primaryCameraEntity = sceneManager.GetSceneObject().GetPrimaryCameraEntity();
						if (primaryCameraEntity)
						{
							if (&primaryCameraEntity.GetComponent<CameraComponent>().GetCamera() != &cc.GetCamera())
							{
								if (primaryCameraEntity.GetComponent<CameraComponent>().Primary == true)
								{
									primaryCameraEntity.GetComponent<CameraComponent>().Primary = false;
									cc.Primary = true;
								}
							}
						}
						else
						{
							cc.Primary = true;
						}
					}
				}

				const char* items[] = { "Perspective", "Orthographic" };
				const char* current_item = cc.item_type;

				if (ImGui::BeginCombo("##combocameratype", current_item))
				{
					for (int n = 0; n <= 1; n++)
					{
						bool is_selected = (current_item == items[n]);
						if (ImGui::Selectable(items[n], is_selected))
						{
							if (items[n] == "Perspective")
							{
								cc.setType(CameraType::PERSPECTIVE);
							}
							else if (items[n] == "Orthographic")
							{
								cc.setType(CameraType::ORTHOGRAPHIC);
							}
						}
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

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