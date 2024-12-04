#include "qepch.h"
#include "DirectX.h"

#include <d3d11.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace QuasarEngine
{
	DirectX::DirectXData DirectX::m_DirectXData = DirectX::DirectXData();

	void DirectX::Init(GLFWwindow* window)
	{
		CreateDeviceD3D(window);
	}

    void DirectX::Clear()
    {
        CleanupDeviceD3D();
    }

	bool DirectX::CreateDeviceD3D(GLFWwindow* window)
	{
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = glfwGetWin32Window(window);
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &DirectX::m_DirectXData.pSwapChain, &DirectX::m_DirectXData.pd3dDevice, &featureLevel, &DirectX::m_DirectXData.pd3dDeviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &DirectX::m_DirectXData.pSwapChain, &DirectX::m_DirectXData.pd3dDevice, &featureLevel, &DirectX::m_DirectXData.pd3dDeviceContext);
        if (res != S_OK)
            return false;

        CreateRenderTarget();
        return true;
	}

    void DirectX::CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        DirectX::m_DirectXData.pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        DirectX::m_DirectXData.pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &DirectX::m_DirectXData.mainRenderTargetView);
        pBackBuffer->Release();
    }

    void DirectX::CleanupRenderTarget()
    {
        if (DirectX::m_DirectXData.mainRenderTargetView) { DirectX::m_DirectXData.mainRenderTargetView->Release(); DirectX::m_DirectXData.mainRenderTargetView = nullptr; }
    }

    void DirectX::CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (DirectX::m_DirectXData.pSwapChain) { DirectX::m_DirectXData.pSwapChain->Release(); DirectX::m_DirectXData.pSwapChain = nullptr; }
        if (DirectX::m_DirectXData.pd3dDeviceContext) { DirectX::m_DirectXData.pd3dDeviceContext->Release(); DirectX::m_DirectXData.pd3dDeviceContext = nullptr; }
        if (DirectX::m_DirectXData.pd3dDevice) { DirectX::m_DirectXData.pd3dDevice->Release(); DirectX::m_DirectXData.pd3dDevice = nullptr; }
    }
}