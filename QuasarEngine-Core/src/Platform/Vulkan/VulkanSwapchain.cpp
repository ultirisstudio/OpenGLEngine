#include "qepch.h"
#include "VulkanSwapchain.h"

#include "VulkanDevice.h"
#include "VulkanContext.h"
#include "VulkanImage.h"

namespace QuasarEngine
{
	vulkanSwapchain VulkanSwapchain::m_VulkanSwapchain = vulkanSwapchain();

	void VulkanSwapchain::CreateSwapchain(uint32_t width, uint32_t height)
	{
		VkExtent2D swapchainExtent = { width, height };
		m_VulkanSwapchain.maxFramesInFlight = 2;

		bool found = false;
		for (uint32_t i = 0; i < VulkanDevice::m_VulkanDevice.swapchainSupport.formats.size(); ++i)
		{
			VkSurfaceFormatKHR format = VulkanDevice::m_VulkanDevice.swapchainSupport.formats[i];
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				m_VulkanSwapchain.imageFormat = format;
				found = true;
				break;
			}
		}

		if (!found)
		{
			m_VulkanSwapchain.imageFormat = VulkanDevice::m_VulkanDevice.swapchainSupport.formats[0];
		}

		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		for (uint32_t i = 0; i < VulkanDevice::m_VulkanDevice.swapchainSupport.presentModes.size(); ++i)
		{
			VkPresentModeKHR mode = VulkanDevice::m_VulkanDevice.swapchainSupport.presentModes[i];
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				presentMode = mode;
				break;
			}
		}

		VulkanDevice::m_VulkanDevice.swapchainSupport = VulkanDevice::QuerySwapChainSupport(VulkanDevice::m_VulkanDevice.physicalDevice);

		if (VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.currentExtent.width != UINT32_MAX)
		{
			swapchainExtent = VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.currentExtent;
		}
		else
		{
			swapchainExtent.width = std::max(VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.minImageExtent.width, std::min(VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.maxImageExtent.width, swapchainExtent.width));
			swapchainExtent.height = std::max(VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.minImageExtent.height, std::min(VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.maxImageExtent.height, swapchainExtent.height));
		}

		uint32_t imageCount = VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.minImageCount + 1;

		VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
		createInfo.surface = VulkanContext::m_VulkanContext.surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = m_VulkanSwapchain.imageFormat.format;
		createInfo.imageColorSpace = m_VulkanSwapchain.imageFormat.colorSpace;
		createInfo.imageExtent = swapchainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (VulkanDevice::m_VulkanDevice.queueFamilyIndices.graphicsFamily.value() != VulkanDevice::m_VulkanDevice.queueFamilyIndices.presentFamily.value())
		{
			uint32_t queueFamilyIndices[] = { VulkanDevice::m_VulkanDevice.queueFamilyIndices.graphicsFamily.value(), VulkanDevice::m_VulkanDevice.queueFamilyIndices.presentFamily.value() };
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = 0;
		}

		createInfo.preTransform = VulkanDevice::m_VulkanDevice.swapchainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_CHECK(vkCreateSwapchainKHR(VulkanDevice::m_VulkanDevice.logicalDevice, &createInfo, VulkanContext::m_VulkanContext.allocator, &m_VulkanSwapchain.handle));

		VulkanContext::m_VulkanContext.currentFrame = 0;

		vkGetSwapchainImagesKHR(VulkanDevice::m_VulkanDevice.logicalDevice, m_VulkanSwapchain.handle, &imageCount, 0);
		m_VulkanSwapchain.images.resize(imageCount);
		vkGetSwapchainImagesKHR(VulkanDevice::m_VulkanDevice.logicalDevice, m_VulkanSwapchain.handle, &imageCount, m_VulkanSwapchain.images.data());

		m_VulkanSwapchain.views.resize(m_VulkanSwapchain.images.size());

		for (uint32_t i = 0; i < m_VulkanSwapchain.images.size(); ++i)
		{
			VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
			createInfo.image = m_VulkanSwapchain.images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_VulkanSwapchain.imageFormat.format;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VK_CHECK(vkCreateImageView(VulkanDevice::m_VulkanDevice.logicalDevice, &createInfo, VulkanContext::m_VulkanContext.allocator, &m_VulkanSwapchain.views[i]))
		}

		if (!VulkanDevice::CheckDepthFormat())
		{
			VulkanDevice::m_VulkanDevice.depthFormat = VK_FORMAT_UNDEFINED;
			Q_FATAL("Depth format not supported");
		}

		VulkanImage::CreateImage(VK_IMAGE_TYPE_2D, swapchainExtent.width, swapchainExtent.height, VulkanDevice::m_VulkanDevice.depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, true, VK_IMAGE_ASPECT_DEPTH_BIT, &m_VulkanSwapchain.depthAttachment);

		Q_DEBUG("Swapchain created");
	}

	void VulkanSwapchain::RecreateSwapchain(uint32_t width, uint32_t height)
	{
		DestroySwapchain();
		CreateSwapchain(width, height);
	}

	void VulkanSwapchain::DestroySwapchain()
	{
		VulkanImage::DestroyImage(&m_VulkanSwapchain.depthAttachment);

		for (uint32_t i = 0; i < m_VulkanSwapchain.images.size(); ++i)
		{
			vkDestroyImageView(VulkanDevice::m_VulkanDevice.logicalDevice, m_VulkanSwapchain.views[i], VulkanContext::m_VulkanContext.allocator);
		}

		vkDestroySwapchainKHR(VulkanDevice::m_VulkanDevice.logicalDevice, m_VulkanSwapchain.handle, VulkanContext::m_VulkanContext.allocator);
	}

	bool VulkanSwapchain::AcquireNextImage(uint64_t timeout, VkSemaphore imageAvailableSemaphore, VkFence fence, uint32_t* outImageIndex)
	{
		VkResult result = vkAcquireNextImageKHR(VulkanDevice::m_VulkanDevice.logicalDevice, VulkanSwapchain::m_VulkanSwapchain.handle, timeout, imageAvailableSemaphore, fence, outImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain(VulkanContext::m_VulkanContext.framebufferWidth, VulkanContext::m_VulkanContext.framebufferHeight);
			return false;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			Q_FATAL("Failed to acquire next image");
			return false;
		}

		return true;
	}

	void VulkanSwapchain::PresentImage(uint32_t imageIndex, VkSemaphore renderFinishedSemaphore)
	{
		VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &VulkanSwapchain::m_VulkanSwapchain.handle;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = 0;

		VkResult result = vkQueuePresentKHR(VulkanDevice::m_VulkanDevice.presentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapchain(VulkanContext::m_VulkanContext.framebufferWidth, VulkanContext::m_VulkanContext.framebufferHeight);
		}
		else if (result != VK_SUCCESS)
		{
			Q_FATAL("Failed to present image");
		}
	}
}
