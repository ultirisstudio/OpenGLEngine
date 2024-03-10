#include "ColliderComponentPanel.h"

#include "imgui.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/ColliderComponent.h>

namespace OpenGLEngine
{
	void ColliderComponentPanel::Render(Entity* entity)
	{
		if (entity->HasComponent<ColliderComponent>())
		{
			auto& cc = entity->GetComponent<ColliderComponent>();

			if (ImGui::TreeNodeEx("Collider", ImGuiTreeNodeFlags_DefaultOpen, "Collider"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<ColliderComponent>();
					}
					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}
	}
}