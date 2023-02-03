#pragma once

#include <DuckEngine/Core/Layer.h>

#include "GameObject.h"
#include "Panels/ContentBrowserPanel.h"

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
	private:
		Camera* m_Camera;
		Framebuffer* m_frameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	private:
		std::shared_ptr<Model> cube;
		std::shared_ptr<Model> sphere;
		std::shared_ptr<Model> plane;

		std::shared_ptr<Model> test;
		std::shared_ptr<Model> temp_model;

		std::vector<GameObject*> m_Objects;

		std::map<std::string, std::shared_ptr<Model>> m_Models;

		FileBrowser m_FileBrowser;
		void OpenExternalFile();

		ContentBrowserPanel m_ContentBrowserPanel;
	private:
		std::shared_ptr<Skybox> m_Skybox;
		std::shared_ptr<Texture> m_Texture;
	private:
		int m_InspectorId;
		bool m_ViewportFocus;
	};
}