#pragma once

#include "OpenGLEngine/Core/Layer.h"

namespace OpenGLEngine
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