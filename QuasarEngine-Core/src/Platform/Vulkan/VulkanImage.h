#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine
{
	class VulkanImage
	{
	public:
		static void CreateImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryFlags, bool createView, VkImageAspectFlags viewAspectFlags, vulkanImage* outImage);
		static void CreateImageView(vulkanImage* image, VkFormat format, VkImageAspectFlags aspectFlags);
		static void DestroyImage(vulkanImage* image);

		//static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		//static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	};
}