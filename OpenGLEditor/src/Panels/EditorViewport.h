#pragma once

#include <memory>

#include "../SceneManager.h"
#include <OpenGLEngine/Renderer/Framebuffer.h>

namespace OpenGLEngine
{
	class EditorViewport
	{
	public:
		EditorViewport();

		void Render(Scene& scene);
		void Update(Scene& scene);

		void OnImGuiRender(SceneManager& sceneManager);
	private:
		std::shared_ptr<Framebuffer> m_EditorFrameBuffer;
		glm::vec2 m_EditorViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		int m_GizmoType = -1;
	};
}
