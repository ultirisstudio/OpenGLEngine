#pragma once

#include "BaseCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace OpenGLEngine
{
	class Camera : public BaseCamera
	{
	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		glm::vec3 m_target;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		glm::vec3* m_Position;
		glm::vec3* m_Rotation;

		float m_minFov;
		float m_maxFov;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };

		friend class EntityPropertiePanel;
		friend class CameraComponent;
		friend class Scene;
	public:
		Camera();
		void Init(glm::vec3& position, glm::vec3& rotation);

		glm::mat4 getViewMatrix() const override;
		const glm::mat4& getProjectionMatrix() const override;

		glm::mat4 GetTransform() override;

		float m_fov;
		float GetFov() const;
		void SetFov(float fov);

		void UpdateCameraVectors();
		void Update();
		void OnResize(float width, float height);
	};
}