#pragma once

#include "QuasarEngine/ImGui/ImGuiLayer.h"

namespace QuasarEngine
{
	class VulkanImGuiLayer : public ImGuiLayer, public Layer
	{
	public:
		VulkanImGuiLayer();
		~VulkanImGuiLayer() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin() override;
		virtual void End() override;
	};
}
