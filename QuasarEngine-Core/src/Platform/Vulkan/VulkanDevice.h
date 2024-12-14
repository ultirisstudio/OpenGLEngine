#pragma once

#include "VulkanTypes.h"

#include <optional>

namespace QuasarEngine {

	class VulkanDevice
	{
	private:
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			std::optional<uint32_t> transferFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
			}
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

	public:
		static vulkanDevice m_VulkanDevice;

		static bool CreateDevice();
		static void DestroyDevice();

	private:
		static bool SelectPhysicalDevice();
		static bool IsDeviceSuitable(VkPhysicalDevice device);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	};
}