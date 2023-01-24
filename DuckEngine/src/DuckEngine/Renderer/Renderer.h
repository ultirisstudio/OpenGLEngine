#pragma once

#include "glm/glm.hpp"

namespace DuckEngine
{
	class Renderer
	{
	public:
		static void Clear();

		static void ClearColor(const glm::vec4& color);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	};
}