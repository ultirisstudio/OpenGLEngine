#include "SceneHierarchy.h"

#include "imgui.h"

#include <OpenGLEngine/Entity/Components/ModelComponent.h>

namespace OpenGLEngine
{
	SceneHierarchy::SceneHierarchy()
	{

	}

	void SceneHierarchy::OnImGuiRender(Scene& scene, Entity* selectedEntity)
	{
		//std::string sceneName = scene.getName().c_str();
		//std::string title = "Scene - " + sceneName;

		ImGui::Begin("Scene");

		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1);

		for (Entity* entity : scene.GetEntityVector())
		{
			UUID id;
			if (selectedEntity)
				id = selectedEntity->GetUUID();
			else
				id = 0;

			ImGuiTreeNodeFlags flags = ((id == entity->GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->GetUUID(), flags, entity->GetName());

			if (ImGui::IsItemClicked())
			{
				selectedEntity = entity;
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Object")) {
					scene.DestroyEntity(*entity);
					selectedEntity = nullptr;
				}
				ImGui::EndPopup();
			}

			if (opened)
			{
				if (entity->HasComponent<ModelComponent>())
				{
					auto& mc = entity->GetComponent<ModelComponent>();

					if (ImGui::TreeNodeEx("Meshes", ImGuiTreeNodeFlags_SpanFullWidth))
					{
						if (mc.GetSubEntities())
						{
							for (auto& subEntity : *mc.GetSubEntities())
							{
								ImGuiTreeNodeFlags flags = ((id == subEntity.GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
								flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
								bool opened = ImGui::TreeNodeEx((void*)(intptr_t)subEntity.GetUUID(), flags, subEntity.GetName());
								if (ImGui::IsItemClicked())
								{
									selectedEntity = &subEntity;
								}
								if (opened)
								{
									ImGui::TreePop();
								}
							}
						}
						ImGui::TreePop();
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