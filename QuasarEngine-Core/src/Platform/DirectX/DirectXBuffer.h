#pragma once

#include <QuasarEngine/Renderer/Buffer.h>

namespace QuasarEngine
{
	class DirectXVertexBuffer : public VertexBuffer
	{
	public:
		DirectXVertexBuffer(uint32_t size);
		DirectXVertexBuffer(const std::vector<float>& vertices, uint32_t size);
		virtual ~DirectXVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual uint32_t GetSize() const { return m_Size; }

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Size;
		BufferLayout m_Layout;
	};

	class DirectXIndexBuffer : public IndexBuffer
	{
	public:
		DirectXIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~DirectXIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}