#include "TerrainComponentPanel.h"

#include <imgui.h>
#include <filesystem>

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>

namespace OpenGLEngine
{
	TerrainComponentPanel::TerrainComponentPanel() : m_NoTexture(*Renderer::m_SceneData.m_ResourceManager.getTexture("Assets/Textures/3d-modeling.png", false))
	{
		
	}

	void TerrainComponentPanel::Render(Entity* entity)
	{
		if (entity->HasComponent<TerrainComponent>())
		{
			auto& tc = entity->GetComponent<TerrainComponent>();

			if (ImGui::TreeNodeEx("Terrain", ImGuiTreeNodeFlags_DefaultOpen, "Terrain"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<TerrainComponent>();
					}
					ImGui::EndPopup();
				}

				ImGui::Text("Heightmap texture: ");

				unsigned int id = tc.GetHeightMapPath() != "" ? Renderer::m_SceneData.m_ResourceManager.getTexture(tc.GetHeightMapPath(), false)->GetID() : m_NoTexture.GetID();

				ImGui::ImageButton((ImTextureID)id, {64.0f, 64.0f}, {0, 1}, {1, 0});
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						tc.SetHeightMap(file);
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::Separator();

				ImGui::Checkbox("Wireframe", &tc.m_PolygonMode);

				if (ImGui::Button("Generate terrain"))
				{
					tc.GenerateTerrain();
				}

				ImGui::TreePop();
			}
		}
	}
}
