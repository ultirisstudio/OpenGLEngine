#include "TerrainComponentPanel.h"

#include <imgui.h>
#include <filesystem>

#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/TerrainComponent.h>

namespace QuasarEngine
{
	TerrainComponentPanel::TerrainComponentPanel()
	{
		if (Renderer::m_SceneData.m_AssetManager->isAssetLoaded("Assets/Icons/no_texture.png"))
		{
			m_NoTexture = Renderer::m_SceneData.m_AssetManager->getAsset<Texture>("Assets/Icons/no_texture.png");
		}
		else
		{
			Renderer::m_SceneData.m_AssetManager->loadAsset("Assets/Icons/no_texture.png");
			m_NoTexture = Renderer::m_SceneData.m_AssetManager->getAsset<Texture>("Assets/Icons/no_texture.png");
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

				if (Renderer::m_SceneData.m_AssetManager->getAsset<Texture>(tc.GetHeightMapPath()))
				{
					id = Renderer::m_SceneData.m_AssetManager->getAsset<Texture>(tc.GetHeightMapPath())->GetID();
				}
				else
				{
					Renderer::m_SceneData.m_AssetManager->loadAsset(tc.GetHeightMapPath());
					std::shared_ptr<Texture> texture = Renderer::m_SceneData.m_AssetManager->getAsset<Texture>(tc.GetHeightMapPath());
					id = texture->GetID();
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
