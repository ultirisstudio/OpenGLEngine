#pragma once

#include <d3d11.h>

struct GLFWwindow;

namespace QuasarEngine
{
	class DirectXRenderer
	{
	public:
		struct DirectXData
		{
			HWND hwnd;

			ID3D11Device* pd3dDevice = nullptr;
			ID3D11DeviceContext* pd3dDeviceContext = nullptr;
			IDXGISwapChain* pSwapChain = nullptr;
			ID3D11RenderTargetView* mainRenderTargetView = nullptr;
		};
		static DirectXData m_DirectXData;

		static void Init(GLFWwindow* window);

		static void Destroy();

		static bool CreateDeviceAndSwapChain();

		static void CleanupRenderTarget();

		static void CreateRenderTarget();

		static void EndFrame();
	};
}