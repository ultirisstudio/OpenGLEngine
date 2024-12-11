#include "qepch.h"
#include "QuasarEngine/Renderer/Buffer.h"
#include "QuasarEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/DirectX/DirectXBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"

namespace QuasarEngine {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
		case RendererAPI::API::DirectX:  return std::make_shared<DirectXVertexBuffer>(size);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanVertexBuffer>(size);
		}

		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const std::vector<float>& vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		case RendererAPI::API::DirectX:  return std::make_shared<DirectXVertexBuffer>(vertices, size);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanVertexBuffer>(vertices, size);
		}

		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, size);
		case RendererAPI::API::DirectX:  return std::make_shared<DirectXIndexBuffer>(indices, size);
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanIndexBuffer>(indices, size);
		}

		return nullptr;
	}

}