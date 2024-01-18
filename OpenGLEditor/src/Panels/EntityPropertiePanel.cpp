#include "EntityPropertiePanel.h"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include <imgui/imgui_internal.h>

#include <filesystem>
#include <sstream>

#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Entity/Components/TransformComponent.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Entity/Components/MeshComponent.h>
#include <OpenGLEngine/Entity/Components/MaterialComponent.h>
#include <OpenGLEngine/Entity/Components/SkyboxComponent.h>
#include <OpenGLEngine/Entity/Components/CameraComponent.h>
#include <OpenGLEngine/Entity/Components/LightComponent.h>
#include <OpenGLEngine/Entity/Components/TerrainComponent.h>

namespace OpenGLEngine
{
	EntityPropertiePanel::EntityPropertiePanel()
	{
		m_ModelTexture = Texture::CreateTexture("Icons/texture_obj.png", false);
	}

	void EntityPropertiePanel::OnImGuiRender(SceneManager& sceneManager)
	{
		ImGui::Begin("Inspector");

		if (sceneManager.getActiveScene().m_SelectedEntity)
		{
			if (sceneManager.getActiveScene().isOnRuntime())
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			Entity* entity = sceneManager.getActiveScene().m_SelectedEntity;

			char* uuid = entity->GetId();

			std::string result;

			std::stringstream sstm;
			sstm << "UUID: " << uuid;
			result = sstm.str();
			ImGui::Text(result.c_str());
			ImGui::Separator();

			std::stringstream sstm2;
			sstm2 << "##" << uuid;
			result = sstm2.str();
			ImGui::InputText(result.c_str(), entity->GetName(), 10);

			ImGui::Separator();

			if (entity->HasComponent<TransformComponent>())
			{
				auto& tc = entity->GetComponent<TransformComponent>();

				if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
				{
					DrawVec3Control("Translation", tc.Position);
					glm::vec3 rotation = glm::degrees(tc.Rotation);
					DrawVec3Control("Rotation", rotation);
					tc.Rotation = glm::radians(rotation);
					DrawVec3Control("Scale", tc.Scale, 1.0f);

					ImGui::TreePop();
				}
			}

			if (entity->HasComponent<CameraComponent>())
			{
				auto& cc = entity->GetComponent<CameraComponent>();

				if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
				{
					ImGui::Text("Camera parameters: ");

					ImGui::Separator();

					bool isActive = sceneManager.getActiveScene().getActiveCamera() == &cc.GetCamera();
					if (ImGui::Checkbox("Active camera", &isActive))
					{
						sceneManager.getActiveScene().setActiveCamera(&cc.GetCamera());
					}

					ImGui::Separator();

					ImGui::Text("FOV: "); ImGui::SameLine();
					ImGui::DragFloat("##FOV", &cc.GetCamera().m_fov, 0.1f, 0.0f, 180.0f, "%.1f");					

					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete Component"))
						{
							entity->RemoveComponent<MaterialComponent>();
						}
						ImGui::EndPopup();
					}
					ImGui::TreePop();
				}
			}

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

			if (entity->HasComponent<MeshComponent>())
			{
				auto& mc = entity->GetComponent<MeshComponent>();

				if (ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen, "Mesh"))
				{
					ImGui::Text("Mesh: "); ImGui::SameLine();
					ImGui::Text(mc.GetName().c_str());

					ImGui::TreePop();
				}
			}

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
					ImGui::ImageButton((ImTextureID)tc.GetEditorHeightMapTexture().GetID(), { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
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

					if (entity->HasComponent<MeshComponent>())
					{
						ImGui::Text("Albedo texture: ");
						ImGui::ImageButton((ImTextureID)mc.GetEditorAlbedoTexture().GetID(), { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
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
						ImGui::ImageButton((ImTextureID)mc.GetEditorNormalTexture().GetID(), { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
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
						ImGui::ImageButton((ImTextureID)mc.GetEditorMetallicTexture().GetID(), { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
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
						ImGui::ImageButton((ImTextureID)mc.GetEditorRoughnessTexture().GetID(), { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
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
						ImGui::ImageButton((ImTextureID)mc.GetEditorAOTexture().GetID(), { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });
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
					}

					ImGui::TreePop();
				}
			}

			if (entity->HasComponent<SkyboxComponent>())
			{
				auto& mc = entity->GetComponent<SkyboxComponent>();

				if (ImGui::TreeNodeEx("Skybox", ImGuiTreeNodeFlags_DefaultOpen, "Skybox"))
				{
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete Component"))
						{
							entity->RemoveComponent<SkyboxComponent>();
						}
						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
			}

			if (entity->HasComponent<LightComponent>())
			{
				auto& lc = entity->GetComponent<LightComponent>();

				if (ImGui::TreeNodeEx("Light", ImGuiTreeNodeFlags_DefaultOpen, "Light"))
				{
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete Component"))
						{
							entity->RemoveComponent<LightComponent>();
						}
						ImGui::EndPopup();
					}

					const char* items[] = { "Directional Light", "Point Light" };
					const char* current_item = lc.item_type;

					if (ImGui::BeginCombo("##combolighttype", current_item))
					{
						for (int n = 0; n <= 1; n++)
						{
							bool is_selected = (current_item == items[n]);
							if (ImGui::Selectable(items[n], is_selected))
							{
								if (items[n] == "Directional Light")
								{
									lc.SetType(LightComponent::LightType::DIRECTIONAL);
								}
								else if (items[n] == "Point Light")
								{
									lc.SetType(LightComponent::LightType::POINT);
								}
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					if (lc.lightType == LightComponent::LightType::DIRECTIONAL)
					{
						ImGui::Text("Directional light color: ");
						ImGui::ColorEdit3("##dirlightcolor", glm::value_ptr(lc.dir_color));
					}
					else if (lc.lightType == LightComponent::LightType::POINT)
					{
						ImGui::Text("Point light color: ");
						ImGui::ColorEdit3("##pointlightcolor", glm::value_ptr(lc.point_color));
						ImGui::Text("Point light constant: ");
						ImGui::InputFloat("##pointlightconstant", &lc.point_constant);
						ImGui::Text("Point light linear: ");
						ImGui::InputFloat("##pointlightlinear", &lc.point_linear);
						ImGui::Text("Point light quadratic: ");
						ImGui::InputFloat("##pointlightquadratic", &lc.point_quadratic);
					}

					ImGui::TreePop();
				}
			}

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent")) {

				if (!entity->HasComponent<TransformComponent>()) {
					if (ImGui::MenuItem("Transform Component")) {
						entity->AddComponent<TransformComponent>();
					}
				}

				if (!entity->HasComponent<ModelComponent>() && !entity->HasComponent<SkyboxComponent>()) {
					if (ImGui::MenuItem("Model Component")) {
						entity->AddComponent<ModelComponent>();
					}
				}

				if (!entity->HasComponent<TerrainComponent>()) {
					if (ImGui::MenuItem("Terrain Component")) {
						entity->AddComponent<TerrainComponent>();
					}
				}

				if (!entity->HasComponent<MaterialComponent>()) {
					if (ImGui::MenuItem("Material Component")) {
						entity->AddComponent<MaterialComponent>();
						entity->GetComponent<MaterialComponent>().InitializeMaterial();
					}
				}

				if (!entity->HasComponent<SkyboxComponent>() && !entity->HasComponent<ModelComponent>()) {
					if (ImGui::MenuItem("Skybox Component")) {
						entity->AddComponent<SkyboxComponent>();
					}
				}

				if (!entity->HasComponent<CameraComponent>()) {
					if (ImGui::MenuItem("Camera Component")) {
						entity->AddComponent<CameraComponent>().Init();
					}
				}

				if (!entity->HasComponent<LightComponent>()) {
					if (ImGui::MenuItem("Directional Light")) {
						entity->AddComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL);
					}
				}

				if (!entity->HasComponent<LightComponent>()) {
					if (ImGui::MenuItem("Point Light Component")) {
						entity->AddComponent<LightComponent>(LightComponent::LightType::POINT);
					}
				}

				ImGui::EndPopup();
			}

			if (sceneManager.getActiveScene().isOnRuntime())
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
		}

		ImGui::End();
	}

	void EntityPropertiePanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
}
