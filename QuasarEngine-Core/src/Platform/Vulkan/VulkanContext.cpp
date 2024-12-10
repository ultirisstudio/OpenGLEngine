#include "qepch.h"
#include "VulkanContext.h"

#include <GLFW/glfw3.h>

#include "QuasarEngine/Core/Log.h"

namespace QuasarEngine
{
	VulkanContext::VulkanContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		
	}

	void VulkanContext::Init()
	{
		//Log::LogAPIInfos((char*)glGetString(GL_VENDOR), (char*)glGetString(GL_RENDERER), (char*)glGetString(GL_VERSION));
	}

	void VulkanContext::BeginFrame()
	{

	}

	void VulkanContext::EndFrame()
	{
		
	}

	void VulkanContext::SetVSync(bool enabled)
	{

	}

	void VulkanContext::SetCursorVisibility(bool visible)
	{
		if (visible)
			glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}