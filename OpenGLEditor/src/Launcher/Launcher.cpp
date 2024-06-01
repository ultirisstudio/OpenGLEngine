#include "Launcher.h"

#include <imgui.h>
#include <imgui_internal.h>


//TOOD Full rework to have a proper launcher, with a panel for creating new projects and another one for opening existing projects
namespace OpenGLEngine
{
	Launcher::Launcher()
	{
		m_ProjectManager = std::make_unique<ProjectManager>();

		m_ProjectManager->CreateNewProject();
	}

	void Launcher::OnAttach()
	{
		
	}

	void Launcher::OnDetach()
	{

	}

	void Launcher::OnUpdate(double dt)
	{

	}

	void Launcher::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			static auto first_time = true;
			if (first_time) {
				ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
				ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

				auto dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking windows into it
				auto dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
				auto dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);

				ImGui::DockBuilderDockWindow("Launcher", dock_id_left);

				ImGui::DockBuilderFinish(dockspace_id);
				first_time = false;
			}
		}

		ImGui::Begin("Launcher", false);

		if (ImGui::Button("Open project"))
		{
			m_ProjectManager->OpenProject();
		}

		m_ProjectManager->OnImGuiRender();

		ImGui::End();

		ImGui::End();
	}

	void Launcher::OnEvent(Event& e)
	{

	}
}