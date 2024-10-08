#pragma once

#include <memory>

#include "../SceneManager.h"
#include "../EditorCamera.h"

#include "SceneHierarchy.h"

#include <OpenGLEngine/Entity/Entity.h>
#include <OpenGLEngine/Renderer/Framebuffer.h>

namespace OpenGLEngine
{
	class EditorViewport
	{
	public:
		EditorViewport();

		void Render(Scene& scene, EditorCamera& camera);
		void Update(EditorCamera& camera);

		void OnImGuiRender(EditorCamera& camera, SceneManager& sceneManager, SceneHierarchy& sceneHierarchy);

		Entity GetHoveredEntity() { return m_HoveredEntity; }

		inline bool const IsViewportFocused() const { return m_ViewportFocused; }
		inline bool const IsViewportHovered() const { return m_ViewportHovered; }
	private:
		std::shared_ptr<Framebuffer> m_EditorFrameBuffer;
		glm::vec2 m_EditorViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_EditorViewportBounds[2];
		glm::vec2 m_WindowTitleBarSize = { 0, 0 };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Entity m_HoveredEntity = {};

		int m_GizmoType = -1;
	};
}
