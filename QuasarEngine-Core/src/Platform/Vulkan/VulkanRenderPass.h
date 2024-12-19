#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine
{
	class VulkanRenderPass
	{
	private:
		vulkanRenderPass m_renderPass;

		vulkanCommandBuffer m_commandBuffer;

		VkFramebuffer m_framebuffer;

	public:
		void CreateRenderPass(float x, float y, float w, float h, float r, float g, float b, float a, float depth, uint32_t stencil);
		void DestroyRenderPass();

		void Begin();
		void End();

		vulkanRenderPass& GetRenderPass() { return m_renderPass; }
	};
}