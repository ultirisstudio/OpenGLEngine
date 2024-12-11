#pragma once

#include "VulkanTypes.h"

#include <QuasarEngine/Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace QuasarEngine {

	class VulkanContext : public GraphicsContext
	{		
	public:
		static vulkanContext m_VulkanContext;

		VulkanContext(GLFWwindow* windowHandle);

		void Init() override;
		void Destroy() override;

		void BeginFrame() override;
		void DrawFrame() override;
		void EndFrame() override;

	private:
		static std::vector<const char*> getRequiredExtensions();

	private:
		GLFWwindow* m_WindowHandle;
	};
}

