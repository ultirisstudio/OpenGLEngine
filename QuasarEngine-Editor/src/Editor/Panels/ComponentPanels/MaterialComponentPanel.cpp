#include "MaterialComponentPanel.h"

#include <imgui/imgui.h>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Entity/Entity.h>
#include <QuasarEngine/Entity/Components/MaterialComponent.h>
#include <QuasarEngine/Resources/Texture2D.h>

namespace QuasarEngine
{
	MaterialComponentPanel::MaterialComponentPanel()
	{
		
	}

	void MaterialComponentPanel::Render(Entity entity)
	{
		unsigned int NO_ID = Renderer::m_SceneData.m_AssetManager->getAsset<Texture2D>("Assets/Icons/no_texture.png")->GetID();
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

				float thumbnailSize = 80.0f;

				bool haveBorder = false;

				ImGui::Columns(2, "c_albedo", haveBorder);
				{
					ImGui::SetColumnOffset(1, 160);

					ImGui::Text("Albedo texture: ");

					bool hasAlbedoTexture = mc.GetMaterial().HasTexture(TextureType::Albedo);

					id = hasAlbedoTexture ? mc.GetMaterial().GetTexture(TextureType::Albedo)->GetID() : NO_ID;

					ImGui::ImageButton("##albedo_texture", (ImTextureID)id, {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
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

					//ImGui::SameLine();
					ImGui::NextColumn();

					if (hasAlbedoTexture)
					{
						const std::string path = mc.GetMaterial().GetSpecification().AlbedoTexture.value();
						const size_t slash = path.find_last_of("/\\");
						const std::string m_SelectedFile = path.substr(slash + 1);

						size_t lastindex = m_SelectedFile.find_last_of(".");
						const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

						ImGui::Text(m_FileName.c_str());

						if (ImGui::Button("Remove##ALBEDO"))
						{
							mc.GetMaterial().ResetTexture(TextureType::Albedo);
						}
					}
					else
					{
						ImGui::Text("Albedo Color: ");
						ImGui::ColorEdit3("##AlbedoColor", glm::value_ptr(mc.GetMaterial().GetAlbedo()));
					}

					ImGui::Columns(1);
				}

				ImGui::Separator();

				ImGui::Columns(2, "c_normal", haveBorder);
				{
					ImGui::SetColumnOffset(1, 160);

					ImGui::Text("Normal texture: ");

					bool hasNormalTexture = mc.GetMaterial().HasTexture(TextureType::Normal);

					id = hasNormalTexture ? mc.GetMaterial().GetTexture(TextureType::Normal)->GetID() : NO_ID;

					ImGui::ImageButton("##normal_texture", (ImTextureID)id, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
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

					//ImGui::SameLine();
					ImGui::NextColumn();

					if (hasNormalTexture)
					{
						const std::string path = mc.GetMaterial().GetSpecification().NormalTexture.value();
						const size_t slash = path.find_last_of("/\\");
						const std::string m_SelectedFile = path.substr(slash + 1);

						size_t lastindex = m_SelectedFile.find_last_of(".");
						const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

						ImGui::Text(m_FileName.c_str());

						if (ImGui::Button("Remove##NORMAL"))
						{
							mc.GetMaterial().ResetTexture(TextureType::Normal);
						}
					}

					ImGui::Columns(1);
				}

				ImGui::Separator();

				ImGui::Columns(2, "c_metallic", haveBorder);
				{
					ImGui::SetColumnOffset(1, 160);

					ImGui::Text("Metallic texture: ");

					bool hasMetallicTexture = mc.GetMaterial().HasTexture(TextureType::Metallic);

					id = hasMetallicTexture ? mc.GetMaterial().GetTexture(TextureType::Metallic)->GetID() : NO_ID;

					ImGui::ImageButton("##metallic_texture", (ImTextureID)id, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
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

					//ImGui::SameLine();
					ImGui::NextColumn();

					if (hasMetallicTexture)
					{
						const std::string path = mc.GetMaterial().GetSpecification().MetallicTexture.value();
						const size_t slash = path.find_last_of("/\\");
						const std::string m_SelectedFile = path.substr(slash + 1);

						size_t lastindex = m_SelectedFile.find_last_of(".");
						const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

						ImGui::Text(m_FileName.c_str());

						if (ImGui::Button("Remove##METALLIC"))
						{
							mc.GetMaterial().ResetTexture(TextureType::Metallic);
						}
					}
					else
					{
						ImGui::Text("Metallic: ");
						ImGui::SliderFloat("##Metallic", &mc.GetMaterial().GetMetallic(), 0.0f, 1.0f);
					}

					ImGui::Columns(1);
				}

				ImGui::Separator();

				ImGui::Columns(2, "c_roughness", haveBorder);
				{
					ImGui::SetColumnOffset(1, 160);

					ImGui::Text("Roughness texture: ");

					bool hasRoughnessTexture = mc.GetMaterial().HasTexture(TextureType::Roughness);

					id = hasRoughnessTexture ? mc.GetMaterial().GetTexture(TextureType::Roughness)->GetID() : NO_ID;

					ImGui::ImageButton("##roughness_texture", (ImTextureID)id, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
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

					//ImGui::SameLine();
					ImGui::NextColumn();

					if (hasRoughnessTexture)
					{
						const std::string path = mc.GetMaterial().GetSpecification().RoughnessTexture.value();
						const size_t slash = path.find_last_of("/\\");
						const std::string m_SelectedFile = path.substr(slash + 1);

						size_t lastindex = m_SelectedFile.find_last_of(".");
						const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

						ImGui::Text(m_FileName.c_str());

						if (ImGui::Button("Remove##ROUGHNESS"))
						{
							mc.GetMaterial().ResetTexture(TextureType::Roughness);
						}
					}
					else
					{
						ImGui::Text("Roughness: ");
						ImGui::SliderFloat("##Roughness", &mc.GetMaterial().GetRoughness(), 0.0f, 1.0f);
					}
					
					ImGui::Columns(1);
				}

				ImGui::Separator();

				ImGui::Columns(2, "c_ao", haveBorder);
				{
					ImGui::SetColumnOffset(1, 160);

					ImGui::Text("AO texture: ");

					bool hasAOTexture = mc.GetMaterial().HasTexture(TextureType::AO);

					id = hasAOTexture ? mc.GetMaterial().GetTexture(TextureType::AO)->GetID() : NO_ID;

					ImGui::ImageButton("##ao_texture", (ImTextureID)id, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
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

					//ImGui::SameLine();
					ImGui::NextColumn();

					if (hasAOTexture)
					{
						const std::string path = mc.GetMaterial().GetSpecification().AOTexture.value();
						const size_t slash = path.find_last_of("/\\");
						const std::string m_SelectedFile = path.substr(slash + 1);

						size_t lastindex = m_SelectedFile.find_last_of(".");
						const std::string m_FileName = m_SelectedFile.substr(0, lastindex);

						ImGui::Text(m_FileName.c_str());

						if (ImGui::Button("Remove##AO"))
						{
							mc.GetMaterial().ResetTexture(TextureType::AO);
						}
					}
					else
					{
						ImGui::Text("AO: ");
						ImGui::SliderFloat("##AO", &mc.GetMaterial().GetAO(), 0.0f, 1.0f);
					}

					ImGui::Columns(1);
				}

				ImGui::TreePop();
			}
		}
	}
}
