#include "MaterialComponentPanel.h"

#include <imgui.h>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/MaterialComponent.h>

namespace QuasarEngine
{
	MaterialComponentPanel::MaterialComponentPanel()
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

	void MaterialComponentPanel::Render(Entity entity)
	{
		if (entity.HasComponent<MaterialComponent>())
		{
			auto& mc = entity.GetComponent<MaterialComponent>();

			if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen, "Material"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity.RemoveComponent<MaterialComponent>();
					}
					ImGui::EndPopup();
				}

				unsigned int id;

				ImGui::Text("Albedo texture: ");

				id = mc.GetMaterial().HasTexture(TextureType::Albedo) ? mc.GetMaterial().GetTexture(TextureType::Albedo)->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.GetMaterial().SetTexture(TextureType::Albedo, file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().HasTexture(TextureType::Albedo))
				{
					if (ImGui::Button("Remove"))
					{
						mc.GetMaterial().ResetTexture(TextureType::Albedo);
					}

					if (mc.GetMaterial().GetSpecification().AlbedoTexture.has_value())
					{
						const std::string path = mc.GetMaterial().GetSpecification().AlbedoTexture.value();
						const size_t slash = path.find_last_of("/\\");
						const std::string m_SelectedFile = path.substr(slash + 1);

						size_t lastindex = m_SelectedFile.find_last_of(".");
						const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

						ImGui::Text(m_FileName.c_str());
					}
				}

				

				ImGui::Text("Albedo Color: ");
				ImGui::ColorEdit3("##AlbedoColor", glm::value_ptr(mc.GetMaterial().GetAlbedo()));

				ImGui::Separator();

				ImGui::Text("Normal texture: ");

				id = mc.GetMaterial().HasTexture(TextureType::Normal) ? mc.GetMaterial().GetTexture(TextureType::Normal)->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.GetMaterial().SetTexture(TextureType::Normal, file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().HasTexture(TextureType::Normal))
				{
					if (ImGui::Button("Remove"))
					{
						mc.GetMaterial().ResetTexture(TextureType::Normal);
					}
				}

				ImGui::Separator();

				ImGui::Text("Metallic texture: ");

				id = mc.GetMaterial().HasTexture(TextureType::Metallic) ? mc.GetMaterial().GetTexture(TextureType::Metallic)->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.GetMaterial().SetTexture(TextureType::Metallic, file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().HasTexture(TextureType::Metallic))
				{
					if (ImGui::Button("Remove"))
					{
						mc.GetMaterial().ResetTexture(TextureType::Metallic);
					}
				}

				ImGui::Text("Metallic: ");
				ImGui::SliderFloat("##Metallic", &mc.GetMaterial().GetMetallic(), 0.0f, 1.0f);

				ImGui::Separator();

				ImGui::Text("Roughness texture: ");

				id = mc.GetMaterial().HasTexture(TextureType::Roughness) ? mc.GetMaterial().GetTexture(TextureType::Roughness)->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.GetMaterial().SetTexture(TextureType::Roughness, file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().HasTexture(TextureType::Roughness))
				{
					if (ImGui::Button("Remove"))
					{
						mc.GetMaterial().ResetTexture(TextureType::Roughness);
					}
				}

				ImGui::Text("Roughness: ");
				ImGui::SliderFloat("##Roughness", &mc.GetMaterial().GetRoughness(), 0.0f, 1.0f);

				ImGui::Separator();

				ImGui::Text("AO texture: ");

				id = mc.GetMaterial().HasTexture(TextureType::AO) ? mc.GetMaterial().GetTexture(TextureType::AO)->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.GetMaterial().SetTexture(TextureType::AO, file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().HasTexture(TextureType::AO))
				{
					if (ImGui::Button("Remove"))
					{
						mc.GetMaterial().ResetTexture(TextureType::AO);
					}
				}

				ImGui::Text("AO: ");
				ImGui::SliderFloat("##AO", &mc.GetMaterial().GetAO(), 0.0f, 1.0f);

				ImGui::TreePop();
			}
		}
	}
}
