#pragma once

#include <OpenGLEngine/Events/Event.h>
#include <OpenGLEngine/Events/MouseEvent.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace OpenGLEngine
{
	class Camera
	{
	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		glm::vec3 m_position;
		glm::vec3 m_target;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };

		glm::uvec2 m_lastMousePos;

		bool m_canMove;

		bool m_moving;
		bool m_rotate;
		bool m_translate;

		float m_MouseX;
		float m_MouseY;

		float m_fov;
		float m_minFov;
		float m_maxFov;

		float m_RotateSensitivity;
		float m_TranslateSensitivity;
		float m_ScrollSensitivity;

		void updateViewMatrix();

	public:
		Camera(const glm::vec3& position);

		glm::mat4 getViewMatrix() const;
		const glm::mat4& getProjectionMatrix() const;

		glm::mat4 GetTransform();
		float getFov() const;

		void freeze() { m_canMove = false; }
		void free() { m_canMove = true; }

		void UpdateCameraVectors();

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		void Update();
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		bool m_CameraFocus;
	};
}