#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine {

	class VulkanDevice
	{
	private:
		struct vulkanDevice
		{
			VkPhysicalDevice physicalDevice;
			VkDevice logicalDevice;

			SwapChainSupportDetails swapchainSupport;

			VkQueue graphicsQueue;
			VkQueue presentQueue;
			VkQueue transferQueue;

			QueueFamilyIndices queueFamilyIndices;

			VkFormat depthFormat;
		};
		vulkanDevice m_device;

	public:
		bool CreateDevice();
		void DestroyDevice();

		bool CheckDepthFormat();

		vulkanDevice& GetDevice() { return m_device; }

	private:
		bool SelectPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	};
}