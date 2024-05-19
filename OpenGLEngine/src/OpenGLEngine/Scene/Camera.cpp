#include "depch.h"
#include <OpenGLEngine/Scene/Camera.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace OpenGLEngine
{
	Camera::Camera()
		: m_viewMatrix(1.0f),
		m_target(0.0f, 0.0f, -1.0f),
		m_up(0.0f, 1.0f, 0.0f),
		m_right(0.0f, 0.0f, 0.0f),
		m_worldUp(0.0f, 1.0f, 0.0f),
		m_Position(nullptr),
		m_Rotation(nullptr),
		m_projectionMatrix(1.0f),
		m_fov(45.0f),
		m_minFov(15.0f),
		m_maxFov(95.0f)
	{

	}

	void Camera::Init(glm::vec3& position, glm::vec3& rotation)
	{
		m_Position = &position;
		m_Rotation = &rotation;
		m_viewMatrix = glm::lookAt(*m_Position, *m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_target = glm::vec3(0.0f, 0.0f, -1.0f);
		m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_projectionMatrix = glm::mat4(1.0f);
		m_fov = 45.0f;
		m_minFov = 15.0f;
		m_maxFov = 95.0f;

		UpdateCameraVectors();
	}

	void Camera::updateViewMatrix()
	{
		m_viewMatrix = glm::lookAt(*m_Position, *m_Position + m_target, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return m_viewMatrix;
	}

	const glm::mat4& Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	glm::mat4 Camera::GetTransform()
	{
		return glm::translate(glm::mat4(1.f), *m_Position) * glm::toMat4(glm::quat(*m_Rotation));
	}

	float Camera::getFov() const
	{
		return m_fov;
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(m_Rotation->y) * cos(m_Rotation->x);
		front.y = sin(m_Rotation->x);
		front.z = sin(m_Rotation->y) * cos(m_Rotation->x);
		m_target = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_target, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_target));
	}

	void Camera::Update()
	{
		m_projectionMatrix = glm::perspective(glm::radians(getFov()), m_ViewportSize.x / m_ViewportSize.y, 0.1f, 1000.0f);

		updateViewMatrix();
		UpdateCameraVectors();
	}

	void Camera::OnResize(float width, float height)
	{
		m_ViewportSize.x = width;
		m_ViewportSize.y = height;
		glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
	}
}