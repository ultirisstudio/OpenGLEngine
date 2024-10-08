#include "TerrainComponentPanel.h"

#include <imgui.h>
#include <filesystem>

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>

namespace OpenGLEngine
{
	TerrainComponentPanel::TerrainComponentPanel()
	{
		if (Renderer::m_SceneData.m_ResourceManager.GetTexture("Assets/Icons/no_texture.png"))
		{
			m_NoTexture = Renderer::m_SceneData.m_ResourceManager.GetTexture("Assets/Icons/no_texture.png");
		}
		else
		{
			TextureSpecification spec;
			spec.flip = true;
			spec.alpha = true;

			m_NoTexture = Renderer::m_SceneData.m_ResourceManager.CreateTexture("Assets/Icons/no_texture.png", spec);
		}
	}

	void TerrainComponentPanel::Render(Entity entity)
	{
		if (entity.HasComponent<TerrainComponent>())
		{
			auto& tc = entity.GetComponent<TerrainComponent>();

			if (ImGui::TreeNodeEx("Terrain", ImGuiTreeNodeFlags_DefaultOpen, "Terrain"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<TerrainComponent>();
					}
					ImGui::EndPopup();
				}

				ImGui::Text("Heightmap texture: ");

				unsigned int id;

				if (Renderer::m_SceneData.m_ResourceManager.GetTexture(tc.GetHeightMapPath()))
				{
					id = Renderer::m_SceneData.m_ResourceManager.GetTexture(tc.GetHeightMapPath())->GetID();
				}
				else
				{
					TextureSpecification spec;
					id = Renderer::m_SceneData.m_ResourceManager.CreateTexture(tc.GetHeightMapPath(), spec)->GetID();
				}

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

				ImGui::SliderInt("Resolution", &tc.rez, 1, 100);
				ImGui::SliderInt("Texture scale", &tc.textureScale, 1, 100);
				ImGui::SliderFloat("Height multiply", &tc.heightMult, 16.0f, 512.0f);

				ImGui::TreePop();
			}
		}
	}
}
