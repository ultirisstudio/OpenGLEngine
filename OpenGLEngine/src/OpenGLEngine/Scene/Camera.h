#pragma once

#include "BaseCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <OpenGLEngine/ECS/Components/TransformComponent.h>

namespace OpenGLEngine
{
	class Camera : public BaseCamera
	{
	private:
		glm::mat4 m_projectionMatrix;

		float m_minFov;
		float m_maxFov;
		float m_fov;

		glm::vec2 m_ViewportSize;

		TransformComponent* m_TransformComponent;

		friend class EntityPropertiePanel;
		friend class CameraComponent;
		friend class Scene;
	public:
		Camera();
		void Init(TransformComponent* transformComponent);

		const glm::mat4 getViewMatrix() const override;
		const glm::mat4& getProjectionMatrix() const override;

		glm::mat4 GetTransform() override;

		float GetFov() const;
		void SetFov(float fov);

		void OnResize(float width, float height);
	};
}