#pragma once

#include "VulkanTypes.h"

namespace QuasarEngine {

	class VulkanDevice
	{
	public:
		static vulkanDevice m_VulkanDevice;

		static bool CreateDevice();
		static void DestroyDevice();


		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		static bool CheckDepthFormat();

	private:
		static bool SelectPhysicalDevice();
		static bool IsDeviceSuitable(VkPhysicalDevice device);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	};
}