#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <memory>

#include <QuasarEngine/Core/Log.h>

namespace QuasarEngine
{
	class VulkanSwapchain;
	class VulkanDevice;
	class VulkanImage;

	struct vulkanContext
	{
		uint32_t framebufferWidth;
		uint32_t framebufferHeight;

		VkInstance instance;
		VkAllocationCallbacks* allocator;

		VkSurfaceKHR surface;

		VkDebugUtilsMessengerEXT debugMessenger;

		std::unique_ptr<VulkanDevice> device;

		std::unique_ptr<VulkanSwapchain> swapchain;

		uint32_t imageIndex;
		uint32_t currentFrame;

		bool recreatingSwapchain;

		int32_t(*findMemoryIndex)(uint32_t typeFilter, uint32_t propertyFlags);
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

#define VK_CHECK(expr)										\
{															\
	Q_ASSERT(expr == VK_SUCCESS, "Vulkan error");			\
}
}