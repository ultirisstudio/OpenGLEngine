#include "BoxColliderComponentPanel.h"

#include "imgui.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/BoxColliderComponent.h>

namespace OpenGLEngine
{
	void BoxColliderComponentPanel::Render(Entity* entity)
	{
		if (entity->HasComponent<BoxColliderComponent>())
		{
			auto& cc = entity->GetComponent<BoxColliderComponent>();

			if (ImGui::TreeNodeEx("Box Collider", ImGuiTreeNodeFlags_DefaultOpen, "Box Collider"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<BoxColliderComponent>();
					}
					ImGui::EndPopup();
				}

				ImGui::Text("Mass: ");
				if (ImGui::DragFloat("##Mass", &cc.mass, 1.0f, 1.0f, 1000.0f))
				{
					cc.UpdateMaterial();
				}

				ImGui::Text("Friction: ");
				if (ImGui::DragFloat("##Friction", &cc.friction, 0.05f, 0.1f, 10.0f))
				{
					cc.UpdateMaterial();
				}

				ImGui::Text("Bounciness: ");
				if (ImGui::DragFloat("##Bounciness", &cc.bounciness, 0.05f, 0.1f, 1.0f))
				{
					cc.UpdateMaterial();
				}

				ImGui::TreePop();
			}
		}
	}
}