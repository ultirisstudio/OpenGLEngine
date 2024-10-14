#pragma once

#include <QuasarEngine.h>
#include <QuasarEngine/Events/Event.h>
#include <QuasarEngine/Events/KeyEvent.h>
#include <QuasarEngine/Events/MouseEvent.h>

#include <QuasarEngine/Resources/Texture.h>

#include <QuasarEngine/Tools/Chronometer.h>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/EntityPropertiePanel.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/EditorViewport.h"
#include "Panels/Viewport.h"

#include "SceneManager.h"

#include <unordered_map>

namespace QuasarEngine
{
	struct EditorSpecification
	{
		std::string EngineExecutablePath;

		std::string ProjectName;
		std::string ProjectPath;
	};

	class Editor : public Layer
	{
	public:
		Editor(const EditorSpecification& spec);
		virtual ~Editor() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(double dt) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	private:
		void InitImGuiStyle();
		void OptionMenu();
	private:
		EditorSpecification m_Specification;

		ContentBrowserPanel m_ContentBrowserPanel;
		EntityPropertiePanel m_EntityPropertiePanel;
		SceneHierarchy m_SceneHierarchy;
		EditorViewport m_EditorViewport;
		Viewport m_Viewport;

		std::unique_ptr<SceneManager> m_SceneManager;
		std::unique_ptr<EditorCamera> m_EditorCamera;

		std::shared_ptr<Texture> m_TextureTest;
	private:
		Chronometer m_Chronometer;
	private:
		bool m_optionMenu = false;
		int m_optionTab = 0;

		std::vector<unsigned int> m_ImGuiColor;
		std::vector<const char*> m_ThemeName;
		std::map<unsigned int, glm::vec4> m_ThemeColor;
	private:
		void CalculateLatency();

		double last_time = Renderer::GetTime();
		int nb_frame = 0;
		int fps = 0;
		int latency = 0;
	};
}