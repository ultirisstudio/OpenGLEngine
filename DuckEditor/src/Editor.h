#pragma once

#include "DuckEngine.h"
#include "GameObject.h"

namespace DuckEngine
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

		void AddGameObject();
	private:
		Camera* m_Camera;
		Framebuffer* m_frameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	private:
		Model model;
		std::vector<GameObject*> m_Objects;
	private:
		int m_InspectorId;
	};
}