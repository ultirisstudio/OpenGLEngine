#include "MaterialCreatorPanel.h"

namespace DuckEngine
{
	MaterialCreatorPanel::MaterialCreatorPanel()
	{
        
	}

	void MaterialCreatorPanel::OnImGuiRender()
	{
        if (showGraphEditor)
        {
            ImGui::Begin("Graph Editor", NULL, 0);
            //GraphEditor::EditOptions(options);
            if (ImGui::Button("Fit all nodes"))
            {
                fit = GraphEditor::Fit_AllNodes;
            }
            ImGui::SameLine();
            if (ImGui::Button("Fit selected nodes"))
            {
                fit = GraphEditor::Fit_SelectedNodes;
            }
            GraphEditor::Show(delegate, options, viewState, true, &fit);

            ImGui::End();
        }
	}
}