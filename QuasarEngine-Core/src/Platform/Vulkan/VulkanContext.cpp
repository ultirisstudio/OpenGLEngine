#include "qepch.h"
#include "VulkanContext.h"

#include <GLFW/glfw3.h>

#include "VulkanDevice.h"

namespace QuasarEngine
{
	vulkanContext VulkanContext::m_VulkanContext = vulkanContext();

	VulkanContext::VulkanContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		
	}

	void VulkanContext::Init()
	{
		VulkanContext::m_VulkanContext.allocator = 0;

		VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		app_info.apiVersion = VK_API_VERSION_1_3;
		app_info.pApplicationName = "Quasar Engine";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName = "Quasar Engine";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

		VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		create_info.pApplicationInfo = &app_info;

		std::vector<const char*> required_extensions = getRequiredExtensions();
		required_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
		create_info.ppEnabledExtensionNames = required_extensions.data();

		std::vector<const char*> required_validation_layers;

#if defined(DEBUG)
		required_validation_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

		create_info.enabledLayerCount = static_cast<uint32_t>(required_validation_layers.size());
		create_info.ppEnabledLayerNames = required_validation_layers.data();

		VK_CHECK(vkCreateInstance(&create_info, VulkanContext::m_VulkanContext.allocator, &VulkanContext::m_VulkanContext.instance));

		Q_INFO("Vulkan instance created successfully");

#if defined(DEBUG)
		uint32_t log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		debug_create_info.messageSeverity = log_severity;
		debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debug_create_info.pfnUserCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) -> VkBool32
			{
				switch (messageSeverity)
				{
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
					Q_DEBUG(pCallbackData->pMessage);
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
					Q_INFO(pCallbackData->pMessage);
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
					Q_WARNING(pCallbackData->pMessage);
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
					Q_ERROR(pCallbackData->pMessage);
					break;
				}
				return VK_FALSE;
			};

		PFN_vkCreateDebugUtilsMessengerEXT func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(VulkanContext::m_VulkanContext.instance, "vkCreateDebugUtilsMessengerEXT"));
		Q_ASSERT(func != nullptr, "Failed to create debug messenger");

		VK_CHECK(func(VulkanContext::m_VulkanContext.instance, &debug_create_info, VulkanContext::m_VulkanContext.allocator, &VulkanContext::m_VulkanContext.debugMessenger));

		Q_DEBUG("Vulkan debug messenger created successfully");
#endif

		CreateSurface();

		if (!VulkanDevice::CreateDevice())
		{
			Q_ERROR("Failed to create Vulkan device");
			return;
		}

		Q_INFO("Vulkan context initialized successfully");
	}

	void VulkanContext::Destroy()
	{
#if defined(_DEBUG)
		Q_DEBUG("Destroying Vulkan debugger...");
		if (VulkanContext::m_VulkanContext.debugMessenger)
		{
			PFN_vkDestroyDebugUtilsMessengerEXT func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(VulkanContext::m_VulkanContext.instance, "vkDestroyDebugUtilsMessengerEXT"));
			func(VulkanContext::m_VulkanContext.instance, VulkanContext::m_VulkanContext.debugMessenger, VulkanContext::m_VulkanContext.allocator);
		}
#endif

		Q_DEBUG("Destroying Vulkan instance...");
		vkDestroyInstance(VulkanContext::m_VulkanContext.instance, VulkanContext::m_VulkanContext.allocator);
	}

	void VulkanContext::BeginFrame()
	{

	}

	void VulkanContext::DrawFrame()
	{
	}

	void VulkanContext::EndFrame()
	{
		
	}

	std::vector<const char*> VulkanContext::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(DEBUG)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		Q_DEBUG("Required extensions:");
		size_t size = extensions.size();
		for (size_t i = 0; i < size; i++)
		{
			Q_DEBUG(extensions[i]);
		}
#endif

		return extensions;
	}

	void VulkanContext::CreateSurface()
	{
		VK_CHECK(glfwCreateWindowSurface(VulkanContext::m_VulkanContext.instance, m_WindowHandle, nullptr, &VulkanContext::m_VulkanContext.surface));

		Q_INFO("Window surface created successfully");
	}
}