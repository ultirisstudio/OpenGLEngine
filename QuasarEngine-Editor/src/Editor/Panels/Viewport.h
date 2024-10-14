#pragma once

#include <memory>

#include "../SceneObject.h"

#include <QuasarEngine/Renderer/Framebuffer.h>

namespace QuasarEngine
{
	class Viewport
	{
	public:
		Viewport();

		void Render(SceneObject& sceneObject);

		void OnImGuiRender(SceneObject& sceneObject);
	private:
		std::shared_ptr<Framebuffer> m_ViewportFrameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
	};
}