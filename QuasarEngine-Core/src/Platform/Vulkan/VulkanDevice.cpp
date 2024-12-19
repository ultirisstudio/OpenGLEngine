#include "qepch.h"


#include "VulkanDevice.h"
#include "VulkanContext.h"
#include "VulkanSwapchain.h"

#include <set>

namespace QuasarEngine
{	
	bool VulkanDevice::CreateDevice()
	{
		if (!SelectPhysicalDevice())
		{
			return false;
		}

		Q_DEBUG("Creating logical device...");

		QueueFamilyIndices indices = FindQueueFamilies(m_device.physicalDevice);

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

		VK_CHECK(vkCreateDevice(m_device.physicalDevice, &createInfo, VulkanContext::m_VulkanContext.allocator, &m_device.logicalDevice))
		
		Q_DEBUG("Logical device created successfully");

		vkGetDeviceQueue(m_device.logicalDevice, indices.graphicsFamily.value(), 0, &m_device.graphicsQueue);
		vkGetDeviceQueue(m_device.logicalDevice, indices.presentFamily.value(), 0, &m_device.presentQueue);
		vkGetDeviceQueue(m_device.logicalDevice, indices.transferFamily.value(), 0, &m_device.transferQueue);

		VkCommandPoolCreateInfo poolInfo { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VK_CHECK(vkCreateCommandPool(m_device.logicalDevice, &poolInfo, VulkanContext::m_VulkanContext.allocator, &m_device.graphicsCommandPool));

		Q_DEBUG("Command pool created successfully");

		return true;
	}

	void VulkanDevice::DestroyDevice()
	{
		m_device.graphicsQueue = 0;
		m_device.presentQueue = 0;
		m_device.transferQueue = 0;

		if (m_device.graphicsCommandPool)
		{
			vkDestroyCommandPool(m_device.logicalDevice, m_device.graphicsCommandPool, VulkanContext::m_VulkanContext.allocator);
			m_device.graphicsCommandPool = 0;
		}

		if (m_device.logicalDevice)
		{
			vkDestroyDevice(m_device.logicalDevice, VulkanContext::m_VulkanContext.allocator);
			m_device.logicalDevice = 0;
		}

		m_device.physicalDevice = 0;
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
			m_device.physicalDevice = physical_devices[0];
			vkGetPhysicalDeviceProperties(m_device.physicalDevice, &device_properties);
		}
		else
		{
			int device_index = 0;

			do
			{
				std::cin >> device_index;
			} while (!IsDeviceSuitable(physical_devices[device_index]));

			m_device.physicalDevice = physical_devices[device_index];
			vkGetPhysicalDeviceProperties(m_device.physicalDevice, &device_properties);
		}

		if (m_device.physicalDevice == VK_NULL_HANDLE)
		{
			Q_ERROR("Failed to select GPU!");
			return false;
		}
		else
		{
			std::stringstream ss;
			ss << device_properties.deviceName << " is selected!";
			Q_DEBUG(ss.str());

			m_device.swapchainSupport = VulkanSwapchain::QuerySwapchainSupport(m_device.physicalDevice);
			m_device.queueFamilyIndices = FindQueueFamilies(m_device.physicalDevice);

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
			SwapChainSupportDetails swapChainSupport = VulkanSwapchain::QuerySwapchainSupport(device);
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

	bool VulkanDevice::CheckDepthFormat()
	{
		const uint64_t candidate_count = 3;
		VkFormat candidates[3] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

		uint8_t flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
		for (uint64_t i = 0; i < candidate_count; i++)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_device.physicalDevice, candidates[i], &props);

			if ((props.linearTilingFeatures & flags) == flags)
			{
				m_device.depthFormat = candidates[i];
				return true;
			}
			else if ((props.optimalTilingFeatures & flags) == flags)
			{
				m_device.depthFormat = candidates[i];
				return true;
			}
		}

		return false;
	}
}
