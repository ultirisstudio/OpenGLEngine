#include "qepch.h"
#include "VulkanBuffer.h"

namespace QuasarEngine
{
	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) : m_Size(size)
	{
		
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<float>& vertices, uint32_t size) : m_Size(size)
	{
		
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		
	}

	void VulkanVertexBuffer::Bind() const
	{
		
	}

	void VulkanVertexBuffer::Unbind() const
	{
		
	}

	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		
	}

	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		
	}

	void VulkanIndexBuffer::Bind() const
	{
		
	}

	void VulkanIndexBuffer::Unbind() const
	{
		
	}
}