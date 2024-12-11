#include "qepch.h"
#include "VulkanImGuiLayer.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <ImGuizmo.h>

#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <QuasarEngine/Core/Application.h>

namespace QuasarEngine
{
	VulkanImGuiLayer::VulkanImGuiLayer() : ImGuiLayer("ImGuiLayer")
	{
	}

	VulkanImGuiLayer::~VulkanImGuiLayer()
	{
		
	}

	void VulkanImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		float fontSize = 14.0f;
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Monocraft.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Monocraft.ttf", fontSize);

		ImGui::StyleColorsRealDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		/*Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo infos;
		ImGui_ImplVulkan_Init(&infos);*/
	}

	void VulkanImGuiLayer::OnDetach()
	{
		/*ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();*/
		ImGui::DestroyContext();
	}

	void VulkanImGuiLayer::Begin()
	{
		/*ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();*/
	}

	void VulkanImGuiLayer::End()
	{
		/*ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}*/
	}
}