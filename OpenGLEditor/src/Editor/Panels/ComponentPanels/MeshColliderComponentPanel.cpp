#include "MeshColliderComponentPanel.h"

#include "imgui.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/MeshColliderComponent.h>

namespace OpenGLEngine
{
	void MeshColliderComponentPanel::Render(Entity* entity)
	{
		if (entity->HasComponent<MeshColliderComponent>())
		{
			auto& cc = entity->GetComponent<MeshColliderComponent>();

			if (ImGui::TreeNodeEx("Mesh Collider", ImGuiTreeNodeFlags_DefaultOpen, "Mesh Collider"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<MeshColliderComponent>();
					}
					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}
	}
}