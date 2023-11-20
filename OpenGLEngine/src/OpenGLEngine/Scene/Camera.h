#pragma once

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

		glm::vec3& m_position;
		glm::vec3 m_target;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;

		float m_fov;
		float m_minFov;
		float m_maxFov;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };

		void updateViewMatrix();

		friend class EditorCamera;
		friend class EntityPropertiePanel;
		friend class CameraComponent;
	public:
		Camera(glm::vec3& position);

		glm::mat4 getViewMatrix() const;
		const glm::mat4& getProjectionMatrix() const;

		glm::mat4 GetTransform();
		float getFov() const;
		void setFov(float fov) { m_fov = fov; }

		void UpdateCameraVectors();
		void Update();
		void OnResize(float width, float height);
	};
}