#pragma once

#include <OpenGLEngine.h>

#include "ProjectManager.h"

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
	private:
		std::unique_ptr<ProjectManager> m_ProjectManager;
	};
}