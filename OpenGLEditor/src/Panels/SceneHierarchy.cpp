#include "SceneHierarchy.h"

#include "imgui.h"

OpenGLEngine::SceneHierarchy::SceneHierarchy()
{

}

void OpenGLEngine::SceneHierarchy::OnImGuiRender(Scene& scene)
{
	//std::string sceneName = scene.getName().c_str();
	//std::string title = "Scene - " + sceneName;

	ImGui::Begin("Scene");

	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 1);

	for (Entity* entity : scene.GetEntityVector())
	{
		std::string id;
		if (scene.m_SelectedEntity)
			id = scene.m_SelectedEntity->GetId();
		else
			id = "-1";

		ImGuiTreeNodeFlags flags = ((id.c_str() == entity->GetId()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entity->GetId(), flags, entity->GetName());

		if (ImGui::IsItemClicked())
		{
			scene.m_SelectedEntity = entity;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Object")) {
				scene.DestroyEntity(*entity);
				scene.m_SelectedEntity = nullptr;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
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
