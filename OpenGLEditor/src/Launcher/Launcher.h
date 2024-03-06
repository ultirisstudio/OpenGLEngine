#pragma once

#include <OpenGLEngine.h>

namespace OpenGLEngine
{
	class Launcher : public Layer
	{
	public:
		Launcher();
		virtual ~Launcher() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(double dt) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	};
}