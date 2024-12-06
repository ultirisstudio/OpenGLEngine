#include "qepch.h"
#include "Platform/DirectX/DirectXRendererAPI.h"

#include "QuasarEngine/Renderer/DrawMode.h"

#include "DirectX.h"

#include <d3d11.h>

namespace QuasarEngine
{
	void DirectXRendererAPI::Init()
	{
		
	}

	void DirectXRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		//DirectX::CleanupRenderTarget();
		//DirectX::m_DirectXData.pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		//DirectX::CreateRenderTarget();

		/*D3D11_VIEWPORT vp;
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = x;
		vp.TopLeftY = y;
		DirectX::m_DirectXData.pd3dDeviceContext->RSSetViewports(1u, &vp);*/
	}

	void DirectXRendererAPI::ClearColor(const glm::vec4& color)
	{
		DirectX::m_DirectXData.pd3dDeviceContext->OMSetRenderTargets(1, &DirectX::m_DirectXData.mainRenderTargetView, nullptr);
		const float clear_color_with_alpha[4] = { color.x, color.y, color.z, color.w };
		DirectX::m_DirectXData.pd3dDeviceContext->ClearRenderTargetView(DirectX::m_DirectXData.mainRenderTargetView, clear_color_with_alpha);
	}

	void DirectXRendererAPI::Clear()
	{
		
	}

	void DirectXRendererAPI::SetDepthTest(bool enabled)
	{
	}

	void DirectXRendererAPI::DrawArrays(DrawMode drawMode, uint32_t size)
	{
		
	}

	void DirectXRendererAPI::DrawElements(DrawMode drawMode, uint32_t count)
	{
		
	}
}