#pragma once

#include "DuckEngine/Events/Event.h"
#include <DuckEngine/Events/MouseEvent.h>
#include <glm/glm.hpp>

namespace DuckEngine
{
	class Camera
	{
	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		glm::vec3 m_position;
		glm::vec3 m_target;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };

		glm::uvec2 m_lastMousePos;
		bool m_moving;

		float m_fov;
		float m_minFov;
		float m_maxFov;

		float m_sensitivity;

		void updateViewMatrix();

	public:
		Camera(const glm::vec3& position, const glm::vec3& target);

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

		glm::vec3 getPosition() const;
		float getFov() const;

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		void setPosition(const glm::vec3& position);
		void setTarget(const glm::vec3& target);
		void setFov(float fov);
		void setMinFov(float minFov);
		void setMaxFov(float maxFov);
		void setSensitivity(float sensitivity);

		void Update();
		void OnEvent(Event& e);
		void OnResize(float width, float height);
	};
}