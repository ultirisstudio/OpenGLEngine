#pragma once

#include <memory>

#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Renderer/Framebuffer.h>

namespace OpenGLEngine
{
	class Viewport
	{
	public:
		Viewport();

		void Render(Scene& scene);

		void OnImGuiRender(Scene& scene);
	private:
		std::shared_ptr<Framebuffer> m_ViewportFrameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	};
}