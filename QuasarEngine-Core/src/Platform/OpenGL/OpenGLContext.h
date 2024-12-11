#pragma once

#include "QuasarEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace QuasarEngine {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void Destroy() override;

		void BeginFrame() override;
		void DrawFrame() override;
		void EndFrame() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}