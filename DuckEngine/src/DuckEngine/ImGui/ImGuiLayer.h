#pragma once

#include "DuckEngine/Core/Layer.h"

namespace DuckEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void Init() override;
	};
}