#include "ScriptComponentPanel.h"

#include <imgui.h>

#include <OpenGLEngine/Scripting/ScriptEngine.h>

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Entity/Components/ScriptComponent.h>

namespace OpenGLEngine
{
    void ScriptComponentPanel::Render(Entity* entity)
    {
		if (entity->HasComponent<ScriptComponent>())
		{
			auto& sc = entity->GetComponent<ScriptComponent>();

			if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_DefaultOpen, "Script"))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component"))
					{
						entity->RemoveComponent<ScriptComponent>();
					}
					ImGui::EndPopup();
				}

				const auto& entityClasses = ScriptEngine::GetEntityClasses();
				std::vector<std::string> keys;
				keys.reserve(entityClasses.size());

				for (const auto& key : entityClasses) {
					keys.push_back(key.first);
				}

				if (ImGui::BeginCombo("##combo", sc.m_Name.c_str()))
				{
					for (int n = 0; n < keys.size(); n++)
					{
						bool is_selected = (sc.m_Name == keys[n]);
						if (ImGui::Selectable(keys[n].c_str(), is_selected))
							sc.m_Name = keys[n];
							if (is_selected)
								ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::TreePop();
			}
		}
    }
}