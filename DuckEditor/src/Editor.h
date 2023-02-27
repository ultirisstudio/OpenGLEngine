#pragma once

#include "DuckEngine.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/EntityPropertiePanel.h"

namespace DuckEngine
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

		void AddGameObject(DEFAULT_OBJECT_TYPE type);
		void AddGameObject(const std::string& file);

		void CalculateLatency();
	private:
		std::shared_ptr<Camera> m_Camera;
		std::shared_ptr<Framebuffer> m_frameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	private:
		std::vector<Entity*> m_Entities;
		Entity* m_SelectedEntity;

		FileBrowser m_FileBrowser;
		void OpenExternalFile();

		ContentBrowserPanel m_ContentBrowserPanel;
		EntityPropertiePanel m_EntityPropertiePanel;
	private:
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	private:
		double last_time = glfwGetTime();
		int nb_frame = 0;
		int fps = 0;
		int latency = 0;
	};
}