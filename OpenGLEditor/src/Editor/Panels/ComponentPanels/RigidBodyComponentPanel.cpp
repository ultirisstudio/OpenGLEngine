#include "RigidBodyComponentPanel.h"

#include "imgui.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/RigidBodyComponent.h>

namespace OpenGLEngine
{
	void RigidBodyComponentPanel::Render(Entity* entity)
	{
		if (entity->HasComponent<RigidBodyComponent>())
		{
			auto& rbc = entity->GetComponent<RigidBodyComponent>();

			if (ImGui::TreeNodeEx("RigidBody", ImGuiTreeNodeFlags_DefaultOpen, "RigidBody"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<RigidBodyComponent>();
					}
					ImGui::EndPopup();
				}

				if (ImGui::Checkbox("Enable Gravity", &rbc.enableGravity))
				{
					rbc.UpdateEnableGravity();
				}

				const char* items[] = { "STATIC", "KINEMATIC", "DYNAMIC" };

				if (ImGui::BeginCombo("##combo", rbc.bodyTypeString.data()))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool is_selected = (rbc.bodyTypeString == items[n]);
						if (ImGui::Selectable(items[n], is_selected))
						{
							rbc.bodyTypeString = items[n];
							rbc.UpdateBodyType();
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::TreePop();
			}
		}
	}
}