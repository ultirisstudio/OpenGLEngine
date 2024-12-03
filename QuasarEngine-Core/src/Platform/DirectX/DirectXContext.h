#pragma once

#include "QuasarEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace QuasarEngine {

	class DirectXContext : public GraphicsContext
	{
	public:
		DirectXContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

		virtual void SetCursorVisibility(bool visible) override;

		void CreateDeviceD3D(HWND hWnd);

		static ID3D11Device* g_pd3dDevice;
		static ID3D11DeviceContext* g_pd3dDeviceContext;
		static IDXGISwapChain* g_pSwapChain;
	private:
		GLFWwindow* m_WindowHandle;
	};

}