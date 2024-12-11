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
	};

	struct vulkanDevice
	{
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	};

#define VK_CHECK(expr)										\
{															\
	Q_ASSERT(expr == VK_SUCCESS, "Vulkan error: " + expr);	\
}
}