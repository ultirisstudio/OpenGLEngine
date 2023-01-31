#pragma once

#include "DuckEngine.h"

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
	private:
		Camera* m_Camera;
		Framebuffer* m_frameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	private:
		Model cube;
		Model sphere;
		Model plane;

		std::vector<GameObject*> m_Objects;

		FileBrowser m_FileBrowser;
	private:
		std::shared_ptr<Skybox> m_Skybox;
		std::shared_ptr<Texture> m_Texture;
	private:
		int m_InspectorId;
	};
}