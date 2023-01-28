#pragma once

#include "DuckEngine.h"

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
	private:
		Camera* m_Camera;
		Framebuffer* m_frameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	private:
		std::shared_ptr<RenderModel> renderModel;
		std::shared_ptr<RenderModel> renderModel_2;
	private:
		float m_Position[3];
		float m_Rotation[3];
		float m_Scale[3];
	};
}