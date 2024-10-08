#include "BoxColliderComponentPanel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/Physics/BoxColliderComponent.h>

namespace OpenGLEngine
{
	void BoxColliderComponentPanel::Render(Entity entity)
	{
		if (entity.HasComponent<BoxColliderComponent>())
		{
			auto& cc = entity.GetComponent<BoxColliderComponent>();

			if (ImGui::TreeNodeEx("Box Collider", ImGuiTreeNodeFlags_DefaultOpen, "Box Collider"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<BoxColliderComponent>();
					}
					ImGui::EndPopup();
				}

				ImGui::Text("Use entity scale: ");
				if (ImGui::Checkbox("##UseEntityScale", &cc.m_UseEntityScale))
				{
					cc.UpdateColliderSize();
				}

				ImGui::Text("Size: ");
				if (ImGui::DragFloat3("##Size", glm::value_ptr(cc.m_Size), 0.1f, 0.1f, 10000.0f))
				{
					cc.UpdateColliderSize();
				}

				if (ImGui::TreeNodeEx("Collider material", ImGuiTreeNodeFlags_DefaultOpen, "Collider material"))
				{
					ImGui::Text("Mass: ");
					if (ImGui::DragFloat("##Mass", &cc.mass, 1.0f, 1.0f, 1000.0f))
					{
						cc.UpdateColliderMaterial();
					}

					ImGui::Text("Friction: ");
					if (ImGui::DragFloat("##Friction", &cc.friction, 0.05f, 0.1f, 10.0f))
					{
						cc.UpdateColliderMaterial();
					}

					ImGui::Text("Bounciness: ");
					if (ImGui::DragFloat("##Bounciness", &cc.bounciness, 0.05f, 0.1f, 1.0f))
					{
						cc.UpdateColliderMaterial();
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
		}
	}
}