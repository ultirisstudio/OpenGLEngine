#pragma once

#include <vulkan/vulkan.h>

#include <QuasarEngine/Core/Log.h>

namespace QuasarEngine
{
	struct vulkanContext
	{
		VkInstance instance;
		VkAllocationCallbacks* allocator;

#if defined(_DEBUG)
		VkDebugUtilsMessengerEXT debugMessenger;
#endif

		VkSurfaceKHR surface;
	};

	struct vulkanDevice
	{
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkQueue transferQueue;
	};

#define VK_CHECK(expr)										\
{															\
	Q_ASSERT(expr == VK_SUCCESS, "Vulkan error");			\
}
}