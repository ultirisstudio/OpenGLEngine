#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine
{
	class VulkanSwapchain
	{
	public:
		static vulkanSwapchain m_VulkanSwapchain;

		static void CreateSwapchain(uint32_t width, uint32_t height);
		static void RecreateSwapchain(uint32_t width, uint32_t height);
		static void DestroySwapchain();
		static bool AcquireNextImage(uint64_t timeout, VkSemaphore imageAvailableSemaphore, VkFence fence, uint32_t* outImageIndex);
		static void PresentImage(uint32_t imageIndex, VkSemaphore renderFinishedSemaphore);
	};
}