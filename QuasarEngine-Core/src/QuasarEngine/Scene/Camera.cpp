#include "qepch.h"
#include <QuasarEngine/Scene/Camera.h>

namespace QuasarEngine
{
	Camera::Camera() : m_projectionMatrix(1.0f), m_fov(45.0f), m_minFov(15.0f), m_maxFov(95.0f), m_ViewportSize(1.0f, 1.0f), m_TransformComponent(nullptr)
	{

	}

	void Camera::Init(TransformComponent* transformComponent)
	{
		m_TransformComponent = transformComponent;
	}

	const glm::mat4 Camera::getViewMatrix() const
	{
		return m_TransformComponent->GetGlobalViewMatrix();
	}

	const glm::mat4& Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	glm::mat4 Camera::GetTransform()
	{
		return m_TransformComponent->GetGlobalTransform();
	}

	float Camera::GetFov() const
	{
		return m_fov;
	}

	void Camera::SetFov(float fov)
	{
		m_fov = fov;
		m_projectionMatrix = glm::perspective(glm::radians(GetFov()), m_ViewportSize.x / m_ViewportSize.y, 0.1f, 1000.0f);
	}

	void Camera::OnResize(float width, float height)
	{
		m_ViewportSize.x = width;
		m_ViewportSize.y = height;
		m_projectionMatrix = glm::perspective(glm::radians(GetFov()), m_ViewportSize.x / m_ViewportSize.y, 0.1f, 1000.0f);
	}
}