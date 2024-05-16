#pragma once

#include <OpenGLEngine.h>
#include <OpenGLEngine/Events/Event.h>
#include <OpenGLEngine/Events/KeyEvent.h>
#include <OpenGLEngine/Events/MouseEvent.h>
#include <OpenGLEngine/Resources/Texture.h>
#include <OpenGLEngine/Resources/ScreenQuad.h>
#include <OpenGLEngine/Tools/Chronometer.h>
#include <OpenGLEngine/Shader/Shader.h>

#include "SceneManager.h"

namespace OpenGLEngine
{
	class Runtime : public Layer
	{
	public:
		Runtime();
		virtual ~Runtime() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(double dt) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		std::unique_ptr<SceneManager> m_SceneManager;
		std::unique_ptr<ScreenQuad> m_ScreenQuad;

		std::shared_ptr<Framebuffer> m_FrameBuffer;

		std::unique_ptr<Shader> m_Shader;

		Chronometer m_Chronometer;

		bool m_ViewportFocused;
		bool m_ViewportHovered;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		void CalculateLatency();
	private:
		double last_time = Renderer::GetTime();
		int nb_frame = 0;
		int fps = 0;
		int latency = 0;
	};
}