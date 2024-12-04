#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

struct GLFWwindow;

namespace QuasarEngine
{
	class DirectX
	{
	public:
		struct DirectXData
		{
			ID3D11Device* pd3dDevice;
			ID3D11DeviceContext* pd3dDeviceContext;
			IDXGISwapChain* pSwapChain;
			ID3D11RenderTargetView* mainRenderTargetView;
		};
		static DirectXData m_DirectXData;

		static void Init(GLFWwindow* window);

		static bool CreateDeviceD3D(GLFWwindow* window);

		static void CreateRenderTarget();
	};
}