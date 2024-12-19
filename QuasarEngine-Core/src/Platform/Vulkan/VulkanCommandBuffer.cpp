#include "qepch.h"
#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"

namespace QuasarEngine
{
	void VulkanCommandBuffer::Allocate(VkCommandPool pool, bool isPrimary)
	{
		VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.commandPool = pool;
		allocInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocInfo.commandBufferCount = 1;
		allocInfo.pNext = nullptr;

		m_commandBuffer.state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
		VK_CHECK(vkAllocateCommandBuffers(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, &allocInfo, &m_commandBuffer.handle));
		m_commandBuffer.state = COMMAND_BUFFER_STATE_READY;
	}

	void VulkanCommandBuffer::Free(VkCommandPool pool)
	{
		vkFreeCommandBuffers(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, pool, 1, &m_commandBuffer.handle);

		m_commandBuffer.handle = 0;
		m_commandBuffer.state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
	}

	void VulkanCommandBuffer::Begin(bool isSingleUse, bool isRenderPassContinue, bool isSimultaneousUse)
	{
		VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		beginInfo.flags = 0;
		if (isSingleUse)
		{
			beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		}
		if (isRenderPassContinue)
		{
			beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		}
		if (isSimultaneousUse)
		{
			beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		}

		VK_CHECK(vkBeginCommandBuffer(m_commandBuffer.handle, &beginInfo));
		m_commandBuffer.state = COMMAND_BUFFER_STATE_RECORDING;
	}

	void VulkanCommandBuffer::End()
	{
		VK_CHECK(vkEndCommandBuffer(m_commandBuffer.handle));

		m_commandBuffer.state = COMMAND_BUFFER_STATE_RECORDING_ENDED;
	}

	void VulkanCommandBuffer::UpdateSubmitted()
	{
		m_commandBuffer.state = COMMAND_BUFFER_STATE_SUBMITTED;
	}

	void VulkanCommandBuffer::Reset()
	{
		//VK_CHECK(vkResetCommandBuffer(m_commandBuffer.handle, 0));

		m_commandBuffer.state = COMMAND_BUFFER_STATE_READY;
	}

	void VulkanCommandBuffer::AllocateAndBeginSingleUse(VkCommandPool pool)
	{
		Allocate(pool, true);
		Begin(true, false, false);
	}

	void VulkanCommandBuffer::EndSingleUse(VkCommandPool pool, VkQueue queue)
	{
		End();

		VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer.handle;

		VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, 0));

		VK_CHECK(vkQueueWaitIdle(queue));

		Free(pool);
	}
}