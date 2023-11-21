#pragma once

#include <OpenGLEngine.h>
#include <OpenGLEngine/Events/Event.h>
#include <OpenGLEngine/Events/KeyEvent.h>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/EntityPropertiePanel.h"
#include "Panels/SceneHierarchy.h"
#include "Panels/EditorViewport.h"
#include "Panels/Viewport.h"

#include <map>

namespace OpenGLEngine
{
	enum DEFAULT_OBJECT_TYPE
	{
		CUBE,
		SPHERE,
		PLANE
	};

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

		void AddGameObject(DEFAULT_OBJECT_TYPE type);
		void AddGameObject(const std::string& file);

		void SaveScene();
		void LoadScene();
		void LoadScene(std::string filePath);

		void OptionMenu();

		void CalculateLatency();
	private:
		std::unique_ptr<Scene> m_Scene;
	private:

		FileBrowser m_FileBrowser;
		void OpenExternalFile();

		ContentBrowserPanel m_ContentBrowserPanel;
		EntityPropertiePanel m_EntityPropertiePanel;
		SceneHierarchy m_SceneHierarchy;
		EditorViewport m_EditorViewport;
		Viewport m_Viewport;
	private:
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	private:
		double last_time = glfwGetTime();
		int nb_frame = 0;
		int fps = 0;
		int latency = 0;
	private:
		bool m_optionMenu = false;
		int m_optionTab = 0;

		std::vector<int> m_ImGuiColor;
		std::vector<const char*> m_ThemeName;
		std::map<int, glm::vec4> m_ThemeColor;
	};
}