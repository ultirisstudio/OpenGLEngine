#include "qepch.h"
#include "DirectXBuffer.h"

namespace QuasarEngine
{
	DirectXVertexBuffer::DirectXVertexBuffer(uint32_t size) : m_Size(size)
	{
		
	}

	DirectXVertexBuffer::DirectXVertexBuffer(const std::vector<float>& vertices, uint32_t size) : m_Size(size)
	{
		
	}

	DirectXVertexBuffer::~DirectXVertexBuffer()
	{
		
	}

	void DirectXVertexBuffer::Bind() const
	{
		
	}

	void DirectXVertexBuffer::Unbind() const
	{
		
	}

	void DirectXVertexBuffer::SetData(const void* data, uint32_t size)
	{
		
	}

	DirectXIndexBuffer::DirectXIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		
	}

	DirectXIndexBuffer::~DirectXIndexBuffer()
	{
		
	}

	void DirectXIndexBuffer::Bind() const
	{
		
	}

	void DirectXIndexBuffer::Unbind() const
	{
		
	}
}