#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine
{
	class VulkanRenderPass
	{
	private:
		enum vulkanCommandBufferState : uint8_t
		{
			COMMAND_BUFFER_STATE_READY,
			COMMAND_BUFFER_STATE_RECORDING,
			COMMAND_BUFFER_STATE_IN_RENDER_PASS,
			COMMAND_BUFFER_STATE_RECORDING_ENDED,
			COMMAND_BUFFER_STATE_SUBMITTED,
			COMMAND_BUFFER_STATE_NOT_ALLOCATED
		};

		struct vulkanCommandBuffer
		{
			VkCommandBuffer handle;

			vulkanCommandBufferState state;
		};

		enum vulkanRenderPassState : uint8_t
		{
			READY,
			RECORDING,
			IN_RENDER_PASS,
			RECORDING_ENDED,
			SUBMITTED,
			NOT_ALLOCATED
		};

		struct vulkanRenderPass
		{
			VkRenderPass handle;

			float x, y, w, h;
			float r, g, b, a;

			float death;
			uint32_t stencil;

			vulkanRenderPassState state;
		};

		vulkanRenderPass m_renderPass;
		vulkanCommandBuffer m_commandBuffer;

		VkFramebuffer m_framebuffer;

	public:
		void CreateRenderPass(float x, float y, float w, float h, float r, float g, float b, float a, float depth, uint32_t stencil);
		void DestroyRenderPass();

		void Begin();
		void End();
	};
}