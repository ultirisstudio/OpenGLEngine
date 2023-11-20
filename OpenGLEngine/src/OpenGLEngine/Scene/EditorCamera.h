#pragma once

#include <OpenGLEngine/Scene/Camera.h>
#include <OpenGLEngine/Events/Event.h>
#include <OpenGLEngine/Events/MouseEvent.h>

namespace OpenGLEngine
{
	class EditorCamera : public Camera
	{
	private:
		glm::vec3 m_EPosition;
		glm::uvec2 m_lastMousePos;

		bool m_canMove;

		bool m_moving;
		bool m_rotate;
		bool m_translate;

		float m_MouseX;
		float m_MouseY;

		float m_RotateSensitivity;
		float m_TranslateSensitivity;
		float m_ScrollSensitivity;
	public:
		EditorCamera(const glm::vec3& position);

		void freeze() { m_canMove = false; }
		void free() { m_canMove = true; }

		void OnEvent(Event& e);

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		bool m_CameraFocus;
	};
}