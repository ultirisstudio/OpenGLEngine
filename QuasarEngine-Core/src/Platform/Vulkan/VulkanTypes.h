#pragma once

#include <vulkan/vulkan.h>

#include <optional>

#include <QuasarEngine/Core/Log.h>

namespace QuasarEngine
{
	struct vulkanContext
	{
		uint32_t framebufferWidth;
		uint32_t framebufferHeight;

		VkInstance instance;
		VkAllocationCallbacks* allocator;

#if defined(_DEBUG)
		VkDebugUtilsMessengerEXT debugMessenger;
#endif

		VkSurfaceKHR surface;

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

	struct vulkanImage
	{
		VkImage handle;
		VkDeviceMemory memory;
		VkImageView view;
		uint32_t width;
		uint32_t height;
	};

	struct vulkanSwapchain
	{
		VkSurfaceFormatKHR imageFormat;
		uint8_t maxFramesInFlight;
		VkSwapchainKHR handle;
		std::vector<VkImage> images;
		std::vector<VkImageView> views;

		vulkanImage depthAttachment;
	};

#define VK_CHECK(expr)										\
{															\
	Q_ASSERT(expr == VK_SUCCESS, "Vulkan error");			\
}
}