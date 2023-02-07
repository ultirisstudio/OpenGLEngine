#include "EntityPropertiePanel.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include <filesystem>

namespace DuckEngine
{
	EntityPropertiePanel::EntityPropertiePanel()
	{
		m_ModelTexture = Renderer::CreateTexture("Icons/texture_obj.png");
		m_WhiteTexture = Renderer::CreateTexture("Assets/Textures/white_texture.jpg");
	}

	void EntityPropertiePanel::OnImGuiRender(Entity* entity)
	{
		ImGui::Text("Information:");

		ImGui::Separator();

		std::stringstream sstm;
		sstm << "Object_" << entity->GetId();
		std::string result = sstm.str();
		ImGui::InputText(result.c_str(), entity->GetName(), 10);

		ImGui::Separator();

		if (entity->HasComponent<TransformComponent>())
		{
			auto& tc = entity->GetComponent<TransformComponent>();

			if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				ImGui::Text("Position: "); ImGui::SameLine();
				ImGui::DragFloat3("##Position", glm::value_ptr(tc.Position), 0.1f, -1000.0f, 1000.0f);
				ImGui::Text("Rotation: "); ImGui::SameLine();
				ImGui::DragFloat3("##Rotation", glm::value_ptr(tc.Rotation), 0.1f, 0.0f, 180.0f);
				ImGui::Text("Scale: "); ImGui::SameLine();
				ImGui::DragFloat3("##Scale", glm::value_ptr(tc.Scale), 0.1f, 100.0f);

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

				ImGui::Text("Diffuse texture: "); //ImGui::SameLine();
				if (entity->HasComponent<MaterialComponent>())
					ImGui::ImageButton((ImTextureID)mc.GetMaterial().GetDiffuseTexture().GetID(), { 64.0f, 64.0f }, { 0, 1 }, { 1, 0 });

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filesys = path;
						std::string file = filesys.string();
						mc.SetMaterial(file);

					}
					ImGui::EndDragDropTarget();
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

		if (entity->HasComponent<RenderComponent>())
		{
			auto& mc = entity->GetComponent<RenderComponent>();

			if (ImGui::TreeNodeEx("Render", ImGuiTreeNodeFlags_DefaultOpen, "Render"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<RenderComponent>();
					}
					ImGui::EndPopup();
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

			if (!entity->HasComponent<ModelComponent>()) {
				if (ImGui::MenuItem("Model Component")) {
					entity->AddComponent<ModelComponent>();
				}
			}

			if (!entity->HasComponent<MaterialComponent>()) {
				if (ImGui::MenuItem("Material Component")) {
					entity->AddComponent<MaterialComponent>("Assets/Textures/white_texture.jpg");
				}
			}

			if (!entity->HasComponent<RenderComponent>()) {
				if (ImGui::MenuItem("Render Component")) {
					entity->AddComponent<RenderComponent>();
				}
			}

			if (!entity->HasComponent<SkyboxComponent>()) {
				if (ImGui::MenuItem("Skybox Component")) {
					entity->AddComponent<SkyboxComponent>("Assets/Models/cube.obj");
				}
			}

			ImGui::EndPopup();
		}
	}
}
