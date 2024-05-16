#include "MaterialComponentPanel.h"

#include <imgui.h>
#include <filesystem>
#include "glm/gtc/type_ptr.hpp"

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>

namespace OpenGLEngine
{
	MaterialComponentPanel::MaterialComponentPanel()
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

	void MaterialComponentPanel::Render(Entity* entity)
	{
		if (entity->HasComponent<MaterialComponent>())
		{
			auto& mc = entity->GetComponent<MaterialComponent>();

			if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen, "Material"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<MaterialComponent>();
					}
					ImGui::EndPopup();
				}

				unsigned int id;

				ImGui::Text("Albedo texture: ");

				id = mc.GetMaterial().hasTexture("albedo") ? mc.GetMaterial().getTexture("albedo")->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.addTexture("albedo", file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().hasTexture("albedo"))
				{
					ImGui::Checkbox("Use Albedo Texture", mc.GetMaterial().getBoolean("albedo").get());
				}

				ImGui::Text("Albedo Color: ");
				ImGui::ColorEdit3("##AlbedoColor", glm::value_ptr(*mc.GetMaterial().getVec3("albedo")));

				ImGui::Separator();

				ImGui::Text("Normal texture: ");

				id = mc.GetMaterial().hasTexture("normal") ? mc.GetMaterial().getTexture("normal")->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.addTexture("normal", file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().hasTexture("normal"))
				{
					ImGui::Checkbox("Use Normal Texture", mc.GetMaterial().getBoolean("normal").get());
				}

				ImGui::Separator();

				ImGui::Text("Metallic texture: ");

				id = mc.GetMaterial().hasTexture("metallic") ? mc.GetMaterial().getTexture("metallic")->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.addTexture("metallic", file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().hasTexture("metallic"))
				{
					ImGui::Checkbox("Use Metallic Texture", mc.GetMaterial().getBoolean("metallic").get());
				}

				ImGui::Text("Metallic: ");
				ImGui::SliderFloat("##Metallic", mc.GetMaterial().getFloat("metallic").get(), 0.0f, 1.0f);

				ImGui::Separator();

				ImGui::Text("Roughness texture: ");

				id = mc.GetMaterial().hasTexture("roughness") ? mc.GetMaterial().getTexture("roughness")->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.addTexture("roughness", file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().hasTexture("roughness"))
				{
					ImGui::Checkbox("Use Roughness Texture", mc.GetMaterial().getBoolean("roughness").get());
				}

				ImGui::Text("Roughness: ");
				ImGui::SliderFloat("##Roughness", mc.GetMaterial().getFloat("roughness").get(), 0.0f, 1.0f);

				ImGui::Separator();

				ImGui::Text("AO texture: ");

				id = mc.GetMaterial().hasTexture("ao") ? mc.GetMaterial().getTexture("ao")->GetID() : m_NoTexture->GetID();

				ImGui::ImageButton((ImTextureID)id, { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.addTexture("ao", file);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				if (mc.GetMaterial().hasTexture("ao"))
				{
					ImGui::Checkbox("Use AO Texture", mc.GetMaterial().getBoolean("ao").get());
				}

				ImGui::Text("AO: ");
				ImGui::SliderFloat("##AO", mc.GetMaterial().getFloat("ao").get(), 0.0f, 1.0f);

				ImGui::TreePop();
			}
		}
	}
}
