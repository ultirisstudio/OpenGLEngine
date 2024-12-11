#include "qepch.h"
#include "DirectXContext.h"

#include <GLFW/glfw3.h>

#include "QuasarEngine/Core/Log.h"

#include "DirectXRenderer.h"

namespace QuasarEngine
{
	DirectXContext::DirectXContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		
	}

	void DirectXContext::Init()
	{
		DirectXRenderer::Init(m_WindowHandle);
	}

	void DirectXContext::Destroy()
	{
		DirectXRenderer::Destroy();
	}

	void DirectXContext::BeginFrame()
	{

	}

	void DirectXContext::DrawFrame()
	{

	}

	void DirectXContext::EndFrame()
	{
		DirectXRenderer::EndFrame();
	}
}