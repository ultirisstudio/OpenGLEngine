#include "qepch.h"
#include "VertexArray.h"

#include "QuasarEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/DirectX/DirectXVertexArray.h"
#include "Platform/Vulkan/VulkanVertexArray.h"

namespace QuasarEngine {

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		case RendererAPI::API::DirectX:  return std::make_shared<DirectXVertexArray>();
		case RendererAPI::API::Vulkan:  return std::make_shared<VulkanVertexArray>();
		}

		return nullptr;
	}
}