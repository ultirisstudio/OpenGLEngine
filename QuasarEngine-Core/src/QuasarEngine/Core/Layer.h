#pragma once

#include "QuasarEngine/Events/Event.h"

namespace QuasarEngine
{
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double dt) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
	protected:
		std::string m_Name;
	};
}