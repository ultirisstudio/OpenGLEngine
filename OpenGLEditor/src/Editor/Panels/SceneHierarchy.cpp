#include "SceneHierarchy.h"

#include "imgui.h"

#include <OpenGLEngine/Entity/Components/ModelComponent.h>

namespace OpenGLEngine
{
	SceneHierarchy::SceneHierarchy() : m_SelectedEntity(nullptr)
	{

	}

	void SceneHierarchy::OnImGuiRender(Scene& scene)
	{
		//std::string sceneName = scene.getName().c_str();
		//std::string title = "Scene - " + sceneName;

		ImGui::Begin("Scene");

		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1);

		for (Entity* entity : scene.GetEntityVector())
		{
			UUID id;
			if (m_SelectedEntity)
				id = m_SelectedEntity->GetUUID();
			else
				id = 0;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | ((id == entity->GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->GetUUID(), flags, entity->GetName());
			ImGui::PopStyleVar();

			ImGui::PushID(entity->GetName());
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Object")) {
					scene.DestroyEntity(*entity);
					m_SelectedEntity = nullptr;
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();

			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = entity;
			}

			if (ImGui::BeginDragDropSource()) {
				ImGui::Text(entity->GetName());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				ImGui::EndDragDropTarget();
			}

			if (opened)
			{
				if (entity->HasComponent<ModelComponent>())
				{
					auto& mc = entity->GetComponent<ModelComponent>();

					if (mc.GetSubEntities())
					{
						for (auto& subEntity : *mc.GetSubEntities())
						{
							ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding | ((id == subEntity.GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf;
							flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
							bool opened = ImGui::TreeNodeEx((void*)(intptr_t)subEntity.GetUUID(), flags, subEntity.GetName());
							if (ImGui::IsItemClicked())
							{
								m_SelectedEntity = &subEntity;
							}
							if (opened)
							{
								ImGui::TreePop();
							}
						}
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::PopStyleVar();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				//std::filesystem::path file = path;
				//AddGameObject(file.string());
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
	}
}