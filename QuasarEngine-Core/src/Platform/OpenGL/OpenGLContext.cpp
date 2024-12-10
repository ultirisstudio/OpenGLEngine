#include "qepch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "QuasarEngine/Core/Log.h"

namespace QuasarEngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
			std::cout << "Failed to initialize GLAD" << std::endl;

		Log::LogAPIInfos((char*)glGetString(GL_VENDOR), (char*)glGetString(GL_RENDERER), (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::BeginFrame()
	{
	}

	void OpenGLContext::EndFrame()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLContext::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	void OpenGLContext::SetCursorVisibility(bool visible)
	{
		if (visible)
			glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}