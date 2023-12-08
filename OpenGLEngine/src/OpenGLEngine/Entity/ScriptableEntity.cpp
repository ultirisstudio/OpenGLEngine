#include "depch.h"
#include "ScriptableEntity.h"

#include <OpenGLEngine/Core/Input.h>
#include <GLFW/glfw3.h>

namespace OpenGLEngine
{
    bool ScriptableEntity::GetKeyPressed(const KeyCode key)
	{
		GLFWwindow* window = m_Window;
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return (state == GLFW_PRESS);
	}
}