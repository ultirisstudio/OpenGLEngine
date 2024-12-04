#pragma once

#include "QuasarEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace QuasarEngine {

	class DirectXContext : public GraphicsContext
	{
	public:
		DirectXContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;

		void SetVSync(bool enabled) override;

		void SetCursorVisibility(bool visible) override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}