#include "qepch.h"
#include "VulkanDevice.h"
#include "VulkanContext.h"

#include <set>

namespace QuasarEngine
{
	vulkanDevice VulkanDevice::m_VulkanDevice = vulkanDevice();
	
	bool VulkanDevice::CreateDevice()
	{
		if (!SelectPhysicalDevice())
		{
			return false;
		}

		Q_DEBUG("Creating logical device...");

		QueueFamilyIndices indices = FindQueueFamilies(m_VulkanDevice.physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfo.flags = 0;
			queueCreateInfo.pNext = 0;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = 1;
		const char* extension_names = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		createInfo.ppEnabledExtensionNames = &extension_names;

		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = 0;

		VK_CHECK(vkCreateDevice(m_VulkanDevice.physicalDevice, &createInfo, VulkanContext::m_VulkanContext.allocator, &m_VulkanDevice.logicalDevice))
		
		Q_DEBUG("Logical device created successfully");

		vkGetDeviceQueue(m_VulkanDevice.logicalDevice, indices.graphicsFamily.value(), 0, &m_VulkanDevice.graphicsQueue);
		vkGetDeviceQueue(m_VulkanDevice.logicalDevice, indices.presentFamily.value(), 0, &m_VulkanDevice.presentQueue);
		vkGetDeviceQueue(m_VulkanDevice.logicalDevice, indices.transferFamily.value(), 0, &m_VulkanDevice.transferQueue);

		return true;
	}

	void VulkanDevice::DestroyDevice()
	{
		Q_DEBUG("Destroying Vulkan device...");

		m_VulkanDevice.graphicsQueue = 0;
		m_VulkanDevice.presentQueue = 0;
		m_VulkanDevice.transferQueue = 0;

		if (m_VulkanDevice.logicalDevice)
		{
			vkDestroyDevice(m_VulkanDevice.logicalDevice, VulkanContext::m_VulkanContext.allocator);
			m_VulkanDevice.logicalDevice = 0;
		}

		m_VulkanDevice.physicalDevice = 0;
	}

	bool VulkanDevice::SelectPhysicalDevice()
	{
		uint32_t physical_device_count = 0;
		VK_CHECK(vkEnumeratePhysicalDevices(VulkanContext::m_VulkanContext.instance, &physical_device_count, nullptr));
		if (physical_device_count == 0)
		{
			Q_ERROR("No valid devices were found.");
			return false;
		}

		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		VK_CHECK(vkEnumeratePhysicalDevices(VulkanContext::m_VulkanContext.instance, &physical_device_count, physical_devices.data()));

		VkPhysicalDeviceProperties device_properties;
		for (uint32_t i = 0; i < physical_device_count; i++)
		{
			std::stringstream state;
			if (IsDeviceSuitable(physical_devices[i]))
				state << "\033[32m" << "Suitable" << "\033[0m";
			else
				state << "\033[31m" << "Not suitable" << "\033[0m";

			vkGetPhysicalDeviceProperties(physical_devices[i], &device_properties);
			std::stringstream ss;
			ss << "Device #" << i << ": " << device_properties.deviceName << " (" << state.str() << ")";
			Q_DEBUG(ss.str());
		}

		if (physical_device_count == 1)
		{
			m_VulkanDevice.physicalDevice = physical_devices[0];
			vkGetPhysicalDeviceProperties(m_VulkanDevice.physicalDevice, &device_properties);
		}
		else
		{
			int device_index = 0;

			do
			{
				std::cin >> device_index;
			} while (!IsDeviceSuitable(physical_devices[device_index]));

			m_VulkanDevice.physicalDevice = physical_devices[device_index];
			vkGetPhysicalDeviceProperties(m_VulkanDevice.physicalDevice, &device_properties);
		}

		if (m_VulkanDevice.physicalDevice == VK_NULL_HANDLE)
		{
			Q_ERROR("Failed to select GPU!");
			return false;
		}
		else
		{
			std::stringstream ss;
			ss << device_properties.deviceName << " is selected!";
			Q_DEBUG(ss.str());

			m_VulkanDevice.swapchainSupport = QuerySwapChainSupport(m_VulkanDevice.physicalDevice);
			m_VulkanDevice.queueFamilyIndices = FindQueueFamilies(m_VulkanDevice.physicalDevice);

			return true;
		}

		return false;
	}

	bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = FindQueueFamilies(device);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions;
		requiredExtensions.insert(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VulkanContext::m_VulkanContext.surface, &presentSupport);

			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				indices.transferFamily = i;
			}

			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.isComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanContext::m_VulkanContext.surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanContext::m_VulkanContext.surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanContext::m_VulkanContext.surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanContext::m_VulkanContext.surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanContext::m_VulkanContext.surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	bool VulkanDevice::CheckDepthFormat()
	{
		const uint64_t candidate_count = 3;
		VkFormat candidates[3] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

		uint8_t flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
		for (uint64_t i = 0; i < candidate_count; i++)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_VulkanDevice.physicalDevice, candidates[i], &props);

			if ((props.linearTilingFeatures & flags) == flags)
			{
				m_VulkanDevice.depthFormat = candidates[i];
				return true;
			}
			else if ((props.optimalTilingFeatures & flags) == flags)
			{
				m_VulkanDevice.depthFormat = candidates[i];
				return true;
			}
		}

		return false;
	}
}
