#include "depch.h"
#include <DuckEngine/Scene/Camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <DuckEngine/Core/Window.h>
#include "DuckEngine/Core/Application.h"
/*
namespace DuckEngine
{
	template <typename T>
	static int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	void Camera::updateViewMatrix()
	{
		m_viewMatrix = glm::lookAt(m_position, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	Camera::Camera(const glm::vec3& position, const glm::vec3& target) :
		m_viewMatrix(glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f))),
		m_position(position),
		m_target(target),
		m_lastMousePos(0),
		m_moving(false),
		m_fov(45.0f),
		m_minFov(15.0f),
		m_maxFov(95.0f),
		m_sensitivity(0.01f),
		m_projectionMatrix(1.0f),
		m_CameraFocus(false)
	{

	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return m_viewMatrix;
	}

	glm::mat4 Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	glm::mat4 Camera::GetTransform()
	{
		return glm::translate(glm::mat4(1.0f), m_position);
	}

	glm::vec3 Camera::getPosition() const
	{
		return m_position;
	}

	float Camera::getFov() const
	{
		return m_fov;
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Camera::OnMouseMoved, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&Camera::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Camera::OnMousePressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Camera::OnMouseReleased, this, std::placeholders::_1));
	}

	bool Camera::OnMouseMoved(MouseMovedEvent& e)
	{
		if (!m_moving)
			return false;

		float sensitivity = m_sensitivity;

		float offsetX = static_cast<int>(m_lastMousePos.x - e.GetX()) * sensitivity;
		float offsetY = static_cast<int>(m_lastMousePos.y - e.GetY()) * sensitivity;

		m_lastMousePos.x = e.GetX();
		m_lastMousePos.y = e.GetY();

		float cosAngle = glm::dot(glm::vec3(-glm::transpose(m_viewMatrix)[2]), glm::vec3(0.0f, 1.0f, 0.0f));
		if (cosAngle * sgn(offsetY) > 0.99f)
			offsetY = 0.0f;

		glm::mat4x4 rotationMatrix(1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, offsetX, glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, offsetY, glm::vec3(glm::transpose(m_viewMatrix)[0]));
		m_position = (rotationMatrix * glm::vec4((m_position - m_target), 1.0f)) + glm::vec4(m_target, 1.0f);

		updateViewMatrix();

		return false;
	}

	bool Camera::OnMousePressed(MouseButtonPressedEvent& e)
	{
		Window::WindowData& data = *(Window::WindowData*)glfwGetWindowUserPointer(reinterpret_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));

		m_lastMousePos.x = data.MousePos.x;
		m_lastMousePos.y = data.MousePos.y;

		if (m_CameraFocus)
			m_moving = true;
		return false;
	}

	bool Camera::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		m_moving = false;
		return false;
	}

	bool Camera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_fov -= e.GetYOffset() * 4.0f;

		if (m_fov < m_minFov)
			m_fov = m_minFov;
		if (m_fov > m_maxFov)
			m_fov = m_maxFov;

		return false;
	}

	void Camera::setPosition(const glm::vec3& position)
	{
		m_position = position;
		updateViewMatrix();
	}

	void Camera::setTarget(const glm::vec3& target)
	{
		m_target = target;
		updateViewMatrix();
	}

	void Camera::setFov(float fov)
	{
		m_fov = fov;
	}

	void Camera::setMinFov(float minFov)
	{
		m_minFov = minFov;
	}

	void Camera::setMaxFov(float maxFov)
	{
		m_maxFov = maxFov;
	}

	void Camera::setSensitivity(float sensitivity)
	{
		m_sensitivity = sensitivity;
	}

	void Camera::OnResize(float width, float height)
	{
		m_ViewportSize.x = width;
		m_ViewportSize.y = height;
	}

	void Camera::Update()
	{
		m_projectionMatrix = glm::perspective(glm::radians(getFov()), m_ViewportSize.x / m_ViewportSize.y, 0.1f, 100.0f);
		glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
		Renderer::UpdateMatrix(m_viewMatrix, m_projectionMatrix);
	}
}*/