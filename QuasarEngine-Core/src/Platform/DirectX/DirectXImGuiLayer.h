#pragma once

#include "QuasarEngine/ImGui/ImGuiLayer.h"

namespace QuasarEngine
{
	class DirectXImGuiLayer : public ImGuiLayer, public Layer
	{
	public:
		DirectXImGuiLayer();
		~DirectXImGuiLayer() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin() override;
		virtual void End() override;
	};
}
