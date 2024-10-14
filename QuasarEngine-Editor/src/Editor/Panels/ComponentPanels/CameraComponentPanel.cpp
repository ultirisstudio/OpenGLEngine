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

				bool isActive = sceneManager.getActiveScene().getActiveCamera() == &cc.GetCamera();
				if (ImGui::Checkbox("Active camera", &isActive))
				{
					sceneManager.getActiveScene().setActiveCamera(&cc.GetCamera());
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