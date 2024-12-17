#pragma once

#include "VulkanTypes.h"

#include "VulkanImage.h"

namespace QuasarEngine
{
	class VulkanImage;

	class VulkanSwapchain
	{
	private:
		struct vulkanSwapchain
		{
			VkSurfaceFormatKHR imageFormat;
			uint8_t maxFramesInFlight;
			VkSwapchainKHR handle;
			std::vector<VkImage> images;
			std::vector<VkImageView> views;

			std::unique_ptr<VulkanImage> depthAttachment;
		};
		vulkanSwapchain m_swapchain;

	public:
		void CreateSwapchain(uint32_t width, uint32_t height);
		void RecreateSwapchain(uint32_t width, uint32_t height);
		void DestroySwapchain();
		bool AcquireNextImage(uint64_t timeout, VkSemaphore imageAvailableSemaphore, VkFence fence, uint32_t* outImageIndex);
		void PresentImage(uint32_t imageIndex, VkSemaphore renderFinishedSemaphore);

		vulkanSwapchain& GetSwapchain() { return m_swapchain; }

		static SwapChainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
	};
}