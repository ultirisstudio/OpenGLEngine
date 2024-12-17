#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine
{
	class VulkanImage
	{
	private:
		struct vulkanImage
		{
			VkImage handle;
			VkDeviceMemory memory;
			VkImageView view;
			uint32_t width;
			uint32_t height;
		};
		vulkanImage m_image;

	public:
		void CreateImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryFlags, bool createView, VkImageAspectFlags viewAspectFlags);
		void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
		void DestroyImage();

		vulkanImage& GetImage() { return m_image; }

		//static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		//static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	};
}