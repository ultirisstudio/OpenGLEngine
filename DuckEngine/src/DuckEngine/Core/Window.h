#pragma once

#include "depch.h"
#include <GLFW/glfw3.h>
#include "DuckEngine/Events/Event.h"

namespace DuckEngine {
	
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;

			WindowData(const std::string& title = "Duck Engine", uint32_t width = 1280, uint32_t height = 720, bool vsync = false)
				: Title(title), Width(width), Height(height), VSync(vsync)
			{}
		};

		Window();
		virtual ~Window();

		void OnUpdate();

		inline unsigned int GetWidth() const { return m_Data.Width; }
		inline unsigned int GetHeight() const { return m_Data.Height; }

		void SetVSync(bool enabled);
		bool IsVSync() const;

		inline virtual void* GetNativeWindow() const { return m_Window; }

		inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
	private:
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		WindowData m_Data;
	};

}