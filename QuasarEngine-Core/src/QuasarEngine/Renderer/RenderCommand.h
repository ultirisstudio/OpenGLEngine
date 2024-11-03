#pragma once

#include "QuasarEngine/Renderer/RendererAPI.h"

namespace QuasarEngine {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void ClearColor(const glm::vec4& color)
		{
			s_RendererAPI->ClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}