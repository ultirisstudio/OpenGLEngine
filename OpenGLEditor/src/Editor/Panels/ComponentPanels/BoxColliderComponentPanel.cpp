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

				ImGui::TreePop();
			}
		}
	}
}