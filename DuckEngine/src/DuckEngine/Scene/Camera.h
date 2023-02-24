#pragma once

#include "DuckEngine/Events/Event.h"
#include <DuckEngine/Events/MouseEvent.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace DuckEngine
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
		glm::vec2 m_ViewportPos = { 1.0f, 1.0f };

		glm::uvec2 m_lastMousePos;
		bool m_moving;

		float m_fov;
		float m_minFov;
		float m_maxFov;

		float m_sensitivity;

		void updateViewMatrix();

	public:
		Camera(const glm::vec3& position);

		glm::mat4 getViewMatrix() const;
		const glm::mat4& getProjectionMatrix() const;

		glm::mat4 GetTransform();
		glm::vec3 getPosition() const;
		float getFov() const;

		void UpdateCameraVectors();

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		void Update();
		void OnEvent(Event& e);
		void OnResize(float width, float height);
		void GetViewportPos(float x, float y);

		bool m_CameraFocus;
	};
}