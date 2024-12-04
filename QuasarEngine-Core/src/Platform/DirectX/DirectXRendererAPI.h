#pragma once

#include "QuasarEngine/Renderer/RendererAPI.h"

namespace QuasarEngine {

	class DirectXRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void ClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawArrays(DrawMode drawMode, uint32_t size) override;
		virtual void DrawElements(DrawMode drawMode, uint32_t count) override;
	};
}
