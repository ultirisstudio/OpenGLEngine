#include "CharacterControllerComponentPanel.h"

#include "imgui/imgui.h"

#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/Gameplay/CharacterControllerComponent.h>

namespace QuasarEngine
{
	void CharacterControllerComponentPanel::Render(Entity entity)
	{
		if (entity.HasComponent<CharacterControllerComponent>())
		{
			auto& cc = entity.GetComponent<CharacterControllerComponent>();

			if (ImGui::TreeNodeEx("Character controller", ImGuiTreeNodeFlags_DefaultOpen, "Character controller"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<CharacterControllerComponent>();
					}
					ImGui::EndPopup();
				}

				if (ImGui::TreeNodeEx("Capsule", ImGuiTreeNodeFlags_DefaultOpen, "Capsule"))
				{
					ImGui::Text("Radius: ");
					if (ImGui::DragFloat("##Radius", &cc.m_Radius, 0.05f, 0.1f, 10.0f))
					{
						cc.UpdateColliderSize();
					}

					ImGui::Text("Height: ");
					if (ImGui::DragFloat("##Height", &cc.m_Height, 0.05f, 0.1f, 10.0f))
					{
						cc.UpdateColliderSize();
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen, "Rigidbody"))
				{
					ImGui::Text("Linear Axis Factor");
					ImGui::Text("X: "); ImGui::SameLine();
					if (ImGui::Checkbox("##LX", &cc.m_LinearAxisFactorX))
					{
						cc.UpdateLinearAxisFactor();
					}
					ImGui::SameLine();
					ImGui::Text("Y: "); ImGui::SameLine();
					if (ImGui::Checkbox("##LY", &cc.m_LinearAxisFactorY))
					{
						cc.UpdateLinearAxisFactor();
					}
					ImGui::SameLine();
					ImGui::Text("Z: "); ImGui::SameLine();
					if (ImGui::Checkbox("##LZ", &cc.m_LinearAxisFactorZ))
					{
						cc.UpdateLinearAxisFactor();
					}

					ImGui::Text("Angular Axis Factor");
					ImGui::Text("X: "); ImGui::SameLine();
					if (ImGui::Checkbox("##AX", &cc.m_AngularAxisFactorX))
					{
						cc.UpdateAngularAxisFactor();
					}
					ImGui::SameLine();
					ImGui::Text("Y: "); ImGui::SameLine();
					if (ImGui::Checkbox("##AY", &cc.m_AngularAxisFactorY))
					{
						cc.UpdateAngularAxisFactor();
					}
					ImGui::SameLine();
					ImGui::Text("Z: "); ImGui::SameLine();
					if (ImGui::Checkbox("##AZ", &cc.m_AngularAxisFactorZ))
					{
						cc.UpdateAngularAxisFactor();
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx("Collider material", ImGuiTreeNodeFlags_DefaultOpen, "Collider material"))
				{
					ImGui::Text("Mass: ");
					if (ImGui::DragFloat("##Mass", &cc.mass, 1.0f, 1.0f, 1000.0f))
					{
						cc.UpdateColliderMaterial();
					}

					ImGui::Text("Friction: ");
					if (ImGui::DragFloat("##Friction", &cc.friction, 0.05f, 0.0f, 1000.0f))
					{
						cc.UpdateColliderMaterial();
					}

					ImGui::Text("Bounciness: ");
					if (ImGui::DragFloat("##Bounciness", &cc.bounciness, 0.05f, 0.0f, 1000.0f))
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