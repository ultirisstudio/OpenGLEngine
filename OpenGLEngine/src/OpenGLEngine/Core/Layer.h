#pragma once

#include "OpenGLEngine/Events/Event.h"

namespace OpenGLEngine
{
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
	protected:
		std::string m_Name;
	};
}