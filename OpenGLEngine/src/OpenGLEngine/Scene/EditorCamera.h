#pragma once

#include <OpenGLEngine/Scene/Camera.h>
#include <OpenGLEngine/Events/Event.h>
#include <OpenGLEngine/Events/MouseEvent.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace OpenGLEngine
{
	class EditorCamera
	{
	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		float m_yaw;
		float m_pitch;

		float m_fov;
		float m_minFov;
		float m_maxFov;

		glm::vec3 m_position;
		glm::vec3 m_target;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;
	private:
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

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };

		void updateViewMatrix();
	public:
		EditorCamera(const glm::vec3& position);

		glm::mat4 getViewMatrix() const;
		const glm::mat4& getProjectionMatrix() const;

		glm::mat4 GetTransform();
		float getFov() const;
		void setFov(float fov) { m_fov = fov; }

		void UpdateCameraVectors();
		void Update();
		void OnResize(float width, float height);
	public:

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