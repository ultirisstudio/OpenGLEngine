#include "depch.h"

#include <OpenGLEngine/Scene/EditorCamera.h>
#include <OpenGLEngine/Core/Window.h>
#include "OpenGLEngine/Core/Application.h"

namespace OpenGLEngine
{
	template <typename T>
	static int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	EditorCamera::EditorCamera(const glm::vec3& position) :
		m_EPosition(position),
		Camera(m_EPosition),
		m_lastMousePos(0),
		m_canMove(true),
		m_moving(false),
		m_RotateSensitivity(0.1f),
		m_TranslateSensitivity(0.04f),

		m_ScrollSensitivity(1.0f),
		m_CameraFocus(false),
		m_rotate(false),
		m_translate(false),
		m_MouseX(0.0f),
		m_MouseY(0.0f)
	{
		
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

		float offsetX = static_cast<int>(m_lastMousePos.x - e.GetX());
		float offsetY = static_cast<int>(m_lastMousePos.y - e.GetY());

		m_lastMousePos.x = e.GetX();
		m_lastMousePos.y = e.GetY();

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