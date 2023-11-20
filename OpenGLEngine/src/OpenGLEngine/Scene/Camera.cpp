#include "depch.h"
#include <OpenGLEngine/Scene/Camera.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace OpenGLEngine
{
	Camera::Camera(const glm::vec3& position) :
		m_viewMatrix(glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
		m_target(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_yaw(-90.0f),
		m_pitch(0.0f),
		m_position(position),
		m_projectionMatrix(1.0f),
		m_fov(45.0f),
		m_minFov(15.0f),
		m_maxFov(95.0f)
	{
		UpdateCameraVectors();
	}

	void Camera::updateViewMatrix()
	{
		m_viewMatrix = glm::lookAt(m_position, m_position + m_target, glm::vec3(0.0f, 1.0f, 0.0f));
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
		//glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(m_pitch, m_yaw, 0)));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_pitch), { 1, 0, 0 }) * glm::rotate(glm::mat4(1.0f), glm::radians(m_yaw), { 0, 1, 0 }) * glm::rotate(glm::mat4(1.0f), 0.0f, { 0, 0, 1 });
		return glm::translate(glm::mat4(1.f), m_position) * rotation;
	}

	float Camera::getFov() const
	{
		return m_fov;
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_target = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_target, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_target));
	}

	void Camera::Update()
	{
		m_projectionMatrix = glm::perspective(glm::radians(getFov()), m_ViewportSize.x / m_ViewportSize.y, 0.1f, 100000.0f);

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