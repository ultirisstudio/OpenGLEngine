#include "depch.h"

#include <OpenGLEngine/Scene/EditorCamera.h>
#include <OpenGLEngine/Core/Window.h>
#include "OpenGLEngine/Core/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace OpenGLEngine
{
	template <typename T>
	static int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	EditorCamera::EditorCamera(const glm::vec3& position) :
		m_viewMatrix(glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))),
		m_projectionMatrix(1.0f),

		m_yaw(-90.0f),
		m_pitch(0.0f),
		m_position(position),

		m_target(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),

		m_fov(45.0f),
		m_minFov(15.0f),
		m_maxFov(95.0f),

		m_lastMousePos(0),
		m_canMove(true),
		m_moving(false),
		m_RotateSensitivity(0.16f),
		m_TranslateSensitivity(0.1f),

		m_ScrollSensitivity(2.0f),
		m_CameraFocus(false),
		m_rotate(false),
		m_translate(false),
		m_MouseX(0.0f),
		m_MouseY(0.0f)
	{
		UpdateCameraVectors();
	}

	void EditorCamera::updateViewMatrix()
	{
		m_viewMatrix = glm::lookAt(m_position, m_position + m_target, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::mat4 EditorCamera::getViewMatrix() const
	{
		return m_viewMatrix;
	}

	const glm::mat4& EditorCamera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	glm::mat4 EditorCamera::GetTransform()
	{
		//glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(m_pitch, m_yaw, 0)));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_pitch), { 1, 0, 0 }) * glm::rotate(glm::mat4(1.0f), glm::radians(m_yaw), { 0, 1, 0 }) * glm::rotate(glm::mat4(1.0f), 0.0f, { 0, 0, 1 });
		return glm::translate(glm::mat4(1.f), m_position) * rotation;
	}

	float EditorCamera::getFov() const
	{
		return m_fov;
	}

	void EditorCamera::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_target = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_target, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_target));
	}

	void EditorCamera::Update()
	{
		m_projectionMatrix = glm::perspective(glm::radians(getFov()), m_ViewportSize.x / m_ViewportSize.y, 0.1f, 100000.0f);

		updateViewMatrix();
		UpdateCameraVectors();
	}

	void EditorCamera::OnResize(float width, float height)
	{
		m_ViewportSize.x = width;
		m_ViewportSize.y = height;
		glViewport(0, 0, static_cast<GLsizei>(m_ViewportSize.x), static_cast<GLsizei>(m_ViewportSize.y));
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&EditorCamera::OnMouseMoved, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&EditorCamera::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&EditorCamera::OnMousePressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&EditorCamera::OnMouseReleased, this, std::placeholders::_1));
	}

	bool EditorCamera::OnMouseMoved(MouseMovedEvent& e)
	{
		if (!m_canMove)
			return false;

		if (!m_moving)
			return false;

		float offsetX = static_cast<float>(m_lastMousePos.x - e.GetX());
		float offsetY = static_cast<float>(m_lastMousePos.y - e.GetY());

		m_lastMousePos.x = static_cast<glm::uint>(e.GetX());
		m_lastMousePos.y = static_cast<glm::uint>(e.GetY());

		if (m_rotate)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;

			m_yaw -= (offsetX * m_RotateSensitivity);
			m_pitch += (offsetY * m_RotateSensitivity);
		}

		if (m_translate)
		{
			glm::mat4 translationMatrix(1.0f);
			translationMatrix = glm::translate(translationMatrix, m_right * (-offsetX * m_TranslateSensitivity));
			translationMatrix = glm::translate(translationMatrix, m_up * (offsetY * m_TranslateSensitivity));

			m_position = (translationMatrix * glm::vec4(m_position, 1.0f));
		}

		updateViewMatrix();
		UpdateCameraVectors();

		return false;
	}

	bool EditorCamera::OnMousePressed(MouseButtonPressedEvent& e)
	{
		Window::WindowData& data = *(Window::WindowData*)glfwGetWindowUserPointer(reinterpret_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));

		m_lastMousePos.x = data.MousePos.x;
		m_lastMousePos.y = data.MousePos.y;

		if (m_CameraFocus)
			m_moving = true;

		m_rotate = (e.GetMouseButton() == 1);
		m_translate = (e.GetMouseButton() == 2);

		return false;
	}

	bool EditorCamera::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		m_moving = false;
		return false;
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (!m_CameraFocus)
			return false;

		glm::mat4 translationMatrix(1.0f);
		translationMatrix = glm::translate(translationMatrix, m_target * (e.GetYOffset() * m_ScrollSensitivity));

		m_position = (translationMatrix * glm::vec4(m_position, 1.0f));

		updateViewMatrix();
		UpdateCameraVectors();

		return false;
	}
}