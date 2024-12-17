#include "qepch.h"
#include "VulkanImage.h"

#include "VulkanContext.h"
#include "VulkanDevice.h"

namespace QuasarEngine
{
	void VulkanImage::CreateImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryFlags, bool createView, VkImageAspectFlags viewAspectFlags)
	{
		m_image.width = width;
		m_image.height = height;

		VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		imageInfo.imageType = type;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 4;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateImage(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, &imageInfo, VulkanContext::m_VulkanContext.allocator, &m_image.handle));
	
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_image.handle, &memRequirements);

		int32_t memoryTypeIndex = VulkanContext::m_VulkanContext.findMemoryIndex(memRequirements.memoryTypeBits, memoryFlags);
		if (memoryTypeIndex == -1)
		{
			Q_ERROR("Failed to find suitable memory type!");
			return;
		}

		VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = memoryTypeIndex;

		VK_CHECK(vkAllocateMemory(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, &allocInfo, VulkanContext::m_VulkanContext.allocator, &m_image.memory));

		VK_CHECK(vkBindImageMemory(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_image.handle, m_image.memory, 0));

		if (createView)
		{
			m_image.view = 0;
			CreateImageView(format, viewAspectFlags);
		}
	}

	void VulkanImage::CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		viewInfo.image = m_image.handle;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;

		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VK_CHECK(vkCreateImageView(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, &viewInfo, VulkanContext::m_VulkanContext.allocator, &m_image.view))
	}

	void VulkanImage::DestroyImage()
	{
		vkDestroyImageView(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_image.view, VulkanContext::m_VulkanContext.allocator);
		vkFreeMemory(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_image.memory, VulkanContext::m_VulkanContext.allocator);
		vkDestroyImage(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, m_image.handle, VulkanContext::m_VulkanContext.allocator);
	}
}