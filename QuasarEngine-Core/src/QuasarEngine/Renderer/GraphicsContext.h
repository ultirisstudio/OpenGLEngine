#pragma once

#include <memory>

namespace QuasarEngine {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void Destroy() = 0;

		virtual void BeginFrame() = 0;
		virtual void DrawFrame() = 0;
		virtual void EndFrame() = 0;

		static std::unique_ptr<GraphicsContext> Create(void* window);
	};
}