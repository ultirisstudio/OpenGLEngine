#pragma once

#include <memory>

namespace QuasarEngine {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual void SetCursorVisibility(bool visible) = 0;

		static std::unique_ptr<GraphicsContext> Create(void* window);
	};
}