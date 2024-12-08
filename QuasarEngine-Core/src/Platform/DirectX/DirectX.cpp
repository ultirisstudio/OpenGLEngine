#include "qepch.h"
#include "DirectX.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace QuasarEngine
{
	DirectX::DirectXData DirectX::m_DirectXData = DirectX::DirectXData();

	void DirectX::Init(GLFWwindow* window)
	{
        DirectX::m_DirectXData.hwnd = glfwGetWin32Window(window);

        CreateDeviceAndSwapChain();
	}

	bool DirectX::CreateDeviceAndSwapChain()
	{
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = DirectX::m_DirectXData.hwnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //swapchain->SetFullscreenState(FALSE, NULL);

        HRESULT res = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &DirectX::m_DirectXData.pSwapChain,
            &DirectX::m_DirectXData.pd3dDevice,
            nullptr,
            &DirectX::m_DirectXData.pd3dDeviceContext
        );

        CreateRenderTarget();

        return true;
	}

    void DirectX::CleanupRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        DirectX::m_DirectXData.pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        DirectX::m_DirectXData.pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &DirectX::m_DirectXData.mainRenderTargetView);
        pBackBuffer->Release();
    }

    void DirectX::CreateRenderTarget()
    {
        ID3D11Resource* pBackBuffer = nullptr;
        DirectX::m_DirectXData.pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

        DirectX::m_DirectXData.pd3dDevice->CreateRenderTargetView(
            pBackBuffer,
            nullptr,
            &DirectX::m_DirectXData.mainRenderTargetView
        );

        pBackBuffer->Release();

        DirectX::m_DirectXData.pd3dDeviceContext->OMSetRenderTargets(1, &DirectX::m_DirectXData.mainRenderTargetView, nullptr);

        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = 800;
        viewport.Height = 600;

        DirectX::m_DirectXData.pd3dDeviceContext->RSSetViewports(1, &viewport);
    }

    void DirectX::EndFrame()
    {
        DirectX::m_DirectXData.pSwapChain->Present(0u, 0u);
    }
}