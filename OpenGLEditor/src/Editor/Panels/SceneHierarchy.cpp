#include "SceneHierarchy.h"

#include "imgui.h"

#include <OpenGLEngine/Core/UUID.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>

namespace OpenGLEngine
{
	SceneHierarchy::SceneHierarchy() : m_SelectedEntity(nullptr)
	{

	}

	void SceneHierarchy::OnImGuiRender(Scene& scene)
	{
		ImGui::Begin("Scene");

		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1);

		for (Entity* entity : scene.GetEntityVector())
		{
			if (entity->m_Parent != UUID::Null())
				continue;

			OnDrawEntityNode(scene, entity);
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

	void SceneHierarchy::OnDrawEntityNode(Scene& scene, Entity* entity)
	{
		UUID id;
		if (m_SelectedEntity)
			id = m_SelectedEntity->GetUUID();
		else
			id = 0;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | ((id == entity->GetUUID()) ? ImGuiTreeNodeFlags_Selected : 0) | ((entity->m_Children.size() != 0 ) ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf);
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->GetUUID(), flags, entity->GetName());
		ImGui::PopStyleVar();
		ImGui::PushID(entity->GetName());
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Object")) {
				m_SelectedEntity = nullptr;
				scene.DestroyEntityByUUID(entity->GetUUID());
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
			std::string entityID_str = std::to_string(entity->GetUUID());
			std::wstring widestr = std::wstring(entityID_str.begin(), entityID_str.end());
			const wchar_t* entityID = widestr.c_str();
			ImGui::SetDragDropPayload("SCENE_DRAG_AND_DROP_ENTITY", entityID, (wcslen(entityID) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_DRAG_AND_DROP_ENTITY"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring ws(path);
				std::string entityID_str(ws.begin(), ws.end());

				UUID entityID = std::stoull(entityID_str);
				
				Entity* sourceEntity = scene.GetEntityByUUID(entityID);
				if (sourceEntity)
				{
					if (sourceEntity->m_Parent != UUID::Null())
					{
						for (auto& child : scene.GetEntityByUUID(sourceEntity->m_Parent)->m_Children)
						{
							if (child == sourceEntity->GetUUID())
							{
								scene.GetEntityByUUID(sourceEntity->m_Parent)->m_Children.erase(std::remove(scene.GetEntityByUUID(sourceEntity->m_Parent)->m_Children.begin(), scene.GetEntityByUUID(sourceEntity->m_Parent)->m_Children.end(), sourceEntity->GetUUID()), scene.GetEntityByUUID(sourceEntity->m_Parent)->m_Children.end());
								break;
							}
						}
					}
					sourceEntity->m_Parent = entity->GetUUID();
					entity->m_Children.push_back(sourceEntity->GetUUID());
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (opened)
		{
			std::vector<UUID> childrens = entity->m_Children;

			if (childrens.size() != 0)
			{
				for (UUID child : childrens)
				{
					Entity* childEntity = scene.GetEntityByUUID(child);
					OnDrawEntityNode(scene, childEntity);
				}
			}

			ImGui::TreePop();
		}
	}
}