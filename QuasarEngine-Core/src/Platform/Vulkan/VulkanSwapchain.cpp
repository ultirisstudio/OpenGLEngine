#include "qepch.h"
#include "VulkanSwapchain.h"

#include "VulkanDevice.h"
#include "VulkanContext.h"
#include "VulkanImage.h"

namespace QuasarEngine
{
	void VulkanSwapchain::CreateSwapchain(uint32_t width, uint32_t height)
	{
		VkExtent2D swapchainExtent = { width, height };
		m_swapchain.maxFramesInFlight = 2;

		bool found = false;
		for (uint32_t i = 0; i < VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.formats.size(); ++i)
		{
			VkSurfaceFormatKHR format = VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.formats[i];
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				m_swapchain.imageFormat = format;
				found = true;
				break;
			}
		}

		if (!found)
		{
			m_swapchain.imageFormat = VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.formats[0];
		}

		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		for (uint32_t i = 0; i < VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.presentModes.size(); ++i)
		{
			VkPresentModeKHR mode = VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.presentModes[i];
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				presentMode = mode;
				break;
			}
		}

		VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport = QuerySwapchainSupport(VulkanContext::m_VulkanContext.device->GetDevice().physicalDevice);

		if (VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.currentExtent.width != UINT32_MAX)
		{
			swapchainExtent = VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.currentExtent;
		}
		else
		{
			swapchainExtent.width = std::max(VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.minImageExtent.width, std::min(VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.maxImageExtent.width, swapchainExtent.width));
			swapchainExtent.height = std::max(VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.minImageExtent.height, std::min(VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.maxImageExtent.height, swapchainExtent.height));
		}

		uint32_t imageCount = VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.minImageCount + 1;

		VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
		createInfo.surface = VulkanContext::m_VulkanContext.surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = m_swapchain.imageFormat.format;
		createInfo.imageColorSpace = m_swapchain.imageFormat.colorSpace;
		createInfo.imageExtent = swapchainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (VulkanContext::m_VulkanContext.device->GetDevice().queueFamilyIndices.graphicsFamily.value() != VulkanContext::m_VulkanContext.device->GetDevice().queueFamilyIndices.presentFamily.value())
		{
			uint32_t queueFamilyIndices[] = { VulkanContext::m_VulkanContext.device->GetDevice().queueFamilyIndices.graphicsFamily.value(), VulkanContext::m_VulkanContext.device->GetDevice().queueFamilyIndices.presentFamily.value() };
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

		createInfo.preTransform = VulkanContext::m_VulkanContext.device->GetDevice().swapchainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		VK_CHECK(vkCreateSwapchainKHR(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, &createInfo, VulkanContext::m_VulkanContext.allocator, &m_swapchain.handle));

		VulkanContext::m_VulkanContext.currentFrame = 0;

		vkGetSwapchainImagesKHR(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_swapchain.handle, &imageCount, 0);
		m_swapchain.images.resize(imageCount);
		vkGetSwapchainImagesKHR(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_swapchain.handle, &imageCount, m_swapchain.images.data());

		m_swapchain.views.resize(m_swapchain.images.size());

		for (uint32_t i = 0; i < m_swapchain.images.size(); ++i)
		{
			VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
			createInfo.image = m_swapchain.images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapchain.imageFormat.format;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VK_CHECK(vkCreateImageView(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, &createInfo, VulkanContext::m_VulkanContext.allocator, &m_swapchain.views[i]))
		}

		if (!VulkanContext::m_VulkanContext.device->CheckDepthFormat())
		{
			VulkanContext::m_VulkanContext.device->GetDevice().depthFormat = VK_FORMAT_UNDEFINED;
			Q_FATAL("Depth format not supported");
		}

		VulkanImage::CreateImage(VK_IMAGE_TYPE_2D, swapchainExtent.width, swapchainExtent.height, VulkanContext::m_VulkanContext.device->GetDevice().depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, true, VK_IMAGE_ASPECT_DEPTH_BIT, &m_swapchain.depthAttachment);

		Q_DEBUG("Swapchain created");
	}

	void VulkanSwapchain::RecreateSwapchain(uint32_t width, uint32_t height)
	{
		DestroySwapchain();
		CreateSwapchain(width, height);
	}

	void VulkanSwapchain::DestroySwapchain()
	{
		VulkanImage::DestroyImage(&m_swapchain.depthAttachment);

		for (uint32_t i = 0; i < m_swapchain.images.size(); ++i)
		{
			vkDestroyImageView(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_swapchain.views[i], VulkanContext::m_VulkanContext.allocator);
		}

		vkDestroySwapchainKHR(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_swapchain.handle, VulkanContext::m_VulkanContext.allocator);
	}

	bool VulkanSwapchain::AcquireNextImage(uint64_t timeout, VkSemaphore imageAvailableSemaphore, VkFence fence, uint32_t* outImageIndex)
	{
		VkResult result = vkAcquireNextImageKHR(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, VulkanSwapchain::m_swapchain.handle, timeout, imageAvailableSemaphore, fence, outImageIndex);

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
		presentInfo.pSwapchains = &m_swapchain.handle;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = 0;

		VkResult result = vkQueuePresentKHR(VulkanContext::m_VulkanContext.device->GetDevice().presentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapchain(VulkanContext::m_VulkanContext.framebufferWidth, VulkanContext::m_VulkanContext.framebufferHeight);
		}
		else if (result != VK_SUCCESS)
		{
			Q_FATAL("Failed to present image");
		}
	}

	SwapChainSupportDetails VulkanSwapchain::QuerySwapchainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanContext::m_VulkanContext.surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanContext::m_VulkanContext.surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanContext::m_VulkanContext.surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanContext::m_VulkanContext.surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanContext::m_VulkanContext.surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}
}
