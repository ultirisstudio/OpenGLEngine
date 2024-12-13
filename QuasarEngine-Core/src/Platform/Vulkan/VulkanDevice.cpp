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

		return false;
	}

	void VulkanDevice::DestroyDevice()
	{

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
		VK_CHECK(vkEnumeratePhysicalDevices(VulkanContext::m_VulkanContext.instance, &physical_device_count, physical_devices.data()));;

		VkPhysicalDeviceProperties device_properties;
		for (uint32_t i = 0; i < physical_device_count; i++)
		{
			std::stringstream state;
			if (IsDeviceSuitable(physical_devices[i]))
				state << "\\033[32m" << "Suitable" << "\\033[0m";
			else
				state << "\\033[31m" << "Not suitable" << "\\033[0m";

			vkGetPhysicalDeviceProperties(physical_devices[i], &device_properties);
			std::stringstream ss;
			ss << "Device #" << i << ": " << device_properties.deviceName << " (" << state.str() << ")";
			Q_INFO(ss.str());
		}

		if (physical_device_count == 1)
		{
			VulkanDevice::m_VulkanDevice.physicalDevice = physical_devices[0];
			vkGetPhysicalDeviceProperties(VulkanDevice::m_VulkanDevice.physicalDevice, &device_properties);
		}
		else
		{
			int device_index = 0;

			do
			{
				std::cin >> device_index;
			} while (!IsDeviceSuitable(physical_devices[device_index]));

			VulkanDevice::m_VulkanDevice.physicalDevice = physical_devices[device_index];
			vkGetPhysicalDeviceProperties(VulkanDevice::m_VulkanDevice.physicalDevice, &device_properties);
		}


		if (VulkanDevice::m_VulkanDevice.physicalDevice == VK_NULL_HANDLE)
			Q_ERROR("Failed to select GPU!");
		else
		{
			std::stringstream ss;
			ss << device_properties.deviceName << " is selected!";
			Q_INFO(ss.str());
		}
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
		requiredExtensions.insert("VK_KHR_SWAPCHAIN_EXTENSION_NAME");

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	VulkanDevice::QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice device)
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

	VulkanDevice::SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device)
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
}

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

