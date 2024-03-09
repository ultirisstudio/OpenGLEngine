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
				ImGui::TreePop();
			}
		}
	}
}