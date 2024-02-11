#pragma once

#include <OpenGLEngine.h>
#include <OpenGLEngine/Events/Event.h>
#include <OpenGLEngine/Events/KeyEvent.h>

#include <OpenGLEngine/Tools/Chronometer.h>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/EntityPropertiePanel.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/EditorViewport.h"
#include "Panels/Viewport.h"

#include "SceneManager.h"
#include "ProjectManager.h"

#include <map>

namespace OpenGLEngine
{
	class Editor : public Layer
	{
	public:
		Editor();
		virtual ~Editor() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		void InitImGuiStyle();
		void OptionMenu();
		void CalculateLatency();
	private:
		ContentBrowserPanel m_ContentBrowserPanel;
		EntityPropertiePanel m_EntityPropertiePanel;
		SceneHierarchy m_SceneHierarchy;
		EditorViewport m_EditorViewport;
		Viewport m_Viewport;

		std::unique_ptr<SceneManager> m_SceneManager;
		std::unique_ptr<ProjectManager> m_ProjectManager;
	private:
		double last_time = Renderer::GetTime();
		int nb_frame = 0;
		int fps = 0;
		int latency = 0;
	private:
		Chronometer m_Chronometer;
	private:
		bool m_optionMenu = false;
		int m_optionTab = 0;

		std::vector<int> m_ImGuiColor;
		std::vector<const char*> m_ThemeName;
		std::map<int, glm::vec4> m_ThemeColor;
	};
}