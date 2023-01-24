#pragma once

#include "DuckEngine/Events/Event.h"

namespace DuckEngine
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