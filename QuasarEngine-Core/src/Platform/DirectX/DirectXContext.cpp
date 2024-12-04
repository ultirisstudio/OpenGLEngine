#include "qepch.h"
#include "Platform/DirectX/DirectXContext.h"

#include <GLFW/glfw3.h>

#include "QuasarEngine/Core/Log.h"

#include "DirectX.h"

namespace QuasarEngine
{
	DirectXContext::DirectXContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		
	}

	void DirectXContext::Init()
	{
		DirectX::Init(m_WindowHandle);

		//Log::LogAPIInfos((char*)glGetString(GL_VENDOR), (char*)glGetString(GL_RENDERER), (char*)glGetString(GL_VERSION));
	}

	void DirectXContext::SwapBuffers()
	{
		
	}

	void DirectXContext::SetVSync(bool enabled)
	{

	}

	void DirectXContext::SetCursorVisibility(bool visible)
	{
		if (visible)
			glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}