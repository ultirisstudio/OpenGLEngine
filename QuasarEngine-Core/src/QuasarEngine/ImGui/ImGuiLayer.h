#pragma once

#include "QuasarEngine/Core/Layer.h"

namespace QuasarEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();

		void Begin();
		void End();
	};
}