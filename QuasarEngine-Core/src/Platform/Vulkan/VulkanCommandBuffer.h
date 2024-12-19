#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine
{
	class VulkanCommandBuffer
	{
	private:
		vulkanCommandBuffer m_commandBuffer;

	public:
		VulkanCommandBuffer() = default;
		~VulkanCommandBuffer() = default;

		void Allocate(VkCommandPool pool, bool isPrimary);
		void Free(VkCommandPool pool);

		void Begin(bool isSingleUse, bool isRenderPassContinue, bool isSimultaneousUse);
		void End();

		void UpdateSubmitted();

		void Reset();

		void AllocateAndBeginSingleUse(VkCommandPool pool);
		void EndSingleUse(VkCommandPool pool, VkQueue queue);

		vulkanCommandBuffer& GetCommandBuffer() { return m_commandBuffer; }
	};
}