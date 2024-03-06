#include "ModelComponentPanel.h"

#include <imgui.h>
#include <filesystem>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>

namespace OpenGLEngine
{
	ModelComponentPanel::ModelComponentPanel()
	{
		m_ModelTexture = Texture::CreateTexture("Icons/texture_obj.png", false);
	}

	void ModelComponentPanel::Render(Entity* entity)
	{
		if (entity->HasComponent<ModelComponent>())
		{
			auto& mc = entity->GetComponent<ModelComponent>();

			if (ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_DefaultOpen, "Model"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<ModelComponent>();
					}
					ImGui::EndPopup();
				}

				ImGui::Text("Model: "); ImGui::SameLine();
				ImGui::ImageButton((ImTextureID)m_ModelTexture->GetID(), { 24.0f, 24.0f }, { 0, 1 }, { 1, 0 });

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();

						mc.SetModel(file);
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::TreePop();
			}
		}
	}
}