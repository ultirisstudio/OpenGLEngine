#include "qepch.h"
#include "VulkanRenderer.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>

namespace QuasarEngine
{
    VulkanRenderer::VulkanData VulkanRenderer::VulkanRenderer::m_VulkanData.m_VulkanData = VulkanRenderer::VulkanData();

	void VulkanRenderer::Init(GLFWwindow* window)
	{
		initVulkan();

		VulkanRenderer::VulkanRenderer::m_VulkanData.m_VulkanData.VulkanRenderer::m_VulkanData.m_instance = (VK_NULL_HANDLE);
		VulkanRenderer::VulkanRenderer::m_VulkanData.m_VulkanData.VulkanRenderer::m_VulkanData.m_callback = (VK_NULL_HANDLE);
		VulkanRenderer::VulkanRenderer::m_VulkanData.m_VulkanData.VulkanRenderer::m_VulkanData.m_physicalDevice = (VK_NULL_HANDLE)
	}

	void VulkanRenderer::Destroy()
	{
		cleanup();
	}

	void VulkanRenderer::DrawFrame()
	{
		vkWaitForFences(VulkanRenderer::m_VulkanData.m_device, 1, &VulkanRenderer::m_VulkanData.m_inFlightFences[VulkanRenderer::m_VulkanData.m_currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_swapChain, UINT64_MAX, VulkanRenderer::m_VulkanData.m_imageAvailableSemaphores[VulkanRenderer::m_VulkanData.m_currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			log("Failed to acquire swap chain image!", MESSAGE_TYPE::ERROR);
		}

		if (VulkanRenderer::m_VulkanData.m_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(VulkanRenderer::m_VulkanData.m_device, 1, &VulkanRenderer::m_VulkanData.m_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		VulkanRenderer::m_VulkanData.m_imagesInFlight[imageIndex] = VulkanRenderer::m_VulkanData.m_inFlightFences[VulkanRenderer::m_VulkanData.m_currentFrame];

		vkResetCommandBuffer(VulkanRenderer::m_VulkanData.m_commandBuffers[imageIndex], /*VkCommandBufferResetFlagBits*/ 0);
		recordCommandBuffer(VulkanRenderer::m_VulkanData.m_commandBuffers[imageIndex], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { VulkanRenderer::m_VulkanData.m_imageAvailableSemaphores[VulkanRenderer::m_VulkanData.m_currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &VulkanRenderer::m_VulkanData.m_commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { VulkanRenderer::m_VulkanData.m_renderFinishedSemaphores[VulkanRenderer::m_VulkanData.m_currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(VulkanRenderer::m_VulkanData.m_device, 1, &VulkanRenderer::m_VulkanData.m_inFlightFences[VulkanRenderer::m_VulkanData.m_currentFrame]);

		if (vkQueueSubmit(VulkanRenderer::m_VulkanData.m_graphicsQueue, 1, &submitInfo, VulkanRenderer::m_VulkanData.m_inFlightFences[VulkanRenderer::m_VulkanData.m_currentFrame]) != VK_SUCCESS)
			log("Failed to submit draw command buffer!", MESSAGE_TYPE::ERROR);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { VulkanRenderer::m_VulkanData.m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(VulkanRenderer::m_VulkanData.m_presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || VulkanRenderer::m_VulkanData.m_framebufferResized)
		{
			VulkanRenderer::m_VulkanData.m_framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			log("Failed to present swap chain image!", MESSAGE_TYPE::ERROR);
		}

		VulkanRenderer::m_VulkanData.m_currentFrame = (VulkanRenderer::m_VulkanData.m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createVertexBuffer();
		createIndexBuffer();
		createCommandBuffers();
		createSyncObjects();
	}

	void VulkanRenderer::cleanup()
	{
		cleanupSwapChain();

		vkDestroyBuffer(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_indexBuffer, nullptr);
		vkFreeMemory(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_indexBufferMemory, nullptr);

		vkDestroyBuffer(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_vertexBuffer, nullptr);
		vkFreeMemory(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_vertexBufferMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_commandPool, nullptr);

		vkDestroyDevice(VulkanRenderer::m_VulkanData.m_device, nullptr);

		if (enableValidationLayers)
			DestroyDebugUtilsMessengerEXT(VulkanRenderer::m_VulkanData.m_instance, VulkanRenderer::m_VulkanData.m_callback, nullptr);

		vkDestroySurfaceKHR(VulkanRenderer::m_VulkanData.m_instance, VulkanRenderer::m_VulkanData.m_surface, nullptr);
		vkDestroyInstance(VulkanRenderer::m_VulkanData.m_instance, nullptr);
	}

	void VulkanRenderer::createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
			log("Validation layers requested, but not available!", MESSAGE_TYPE::ERROR);
		else
			log("Validation layers available", MESSAGE_TYPE::INFO);

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &VulkanRenderer::m_VulkanData.m_instance) != VK_SUCCESS)
			log("Failed to create Vulkan instance!", MESSAGE_TYPE::ERROR);
		else
			log("Vulkan instance created successfully", MESSAGE_TYPE::INFO);

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> debugExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, debugExtensions.data());

		if (enableValidationLayers)
		{
			log("Available extensions :", MESSAGE_TYPE::INFO);

			for (const auto& extension : debugExtensions)
				std::cout << "\t" << extension.extensionName << std::endl;
		}
	}

	void VulkanRenderer::setupDebugMessenger() {
		if (!enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(VulkanRenderer::m_VulkanData.m_instance, &createInfo, nullptr, &VulkanRenderer::m_VulkanData.m_callback) != VK_SUCCESS)
			log("Failed to set up debug messenger!", MESSAGE_TYPE::ERROR);
	}

	void VulkanRenderer::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(VulkanRenderer::m_VulkanData.m_instance, &deviceCount, nullptr);

		if (deviceCount == 0)
			log("Failed to find GPUs with Vulkan support!", MESSAGE_TYPE::ERROR);
		else
			log("GPUs with Vulkan support found", MESSAGE_TYPE::INFO);

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(VulkanRenderer::m_VulkanData.m_instance, &deviceCount, devices.data());

		VkPhysicalDeviceProperties deviceProperties;
		for (int i = 0; i < deviceCount; i++)
		{
			std::stringstream state;
			if (isDeviceSuitable(devices[i]))
				state << GREEN << "Suitable" << RESET;
			else
				state << RED << "Not suitable" << RESET;

			vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
			std::stringstream ss;
			ss << "Device #" << i << ": " << deviceProperties.deviceName << " (" << state.str() << ")";
			log(ss.str(), MESSAGE_TYPE::INFO);
		}

		if (deviceCount == 1)
		{
			VulkanRenderer::m_VulkanData.m_physicalDevice = devices[0];
			vkGetPhysicalDeviceProperties(VulkanRenderer::m_VulkanData.m_physicalDevice, &deviceProperties);
		}
		else
		{
			int deviceIndex = 0;

			do
			{
				std::cin >> deviceIndex;
			} while (!isDeviceSuitable(devices[deviceIndex]));

			VulkanRenderer::m_VulkanData.m_physicalDevice = devices[deviceIndex];
			vkGetPhysicalDeviceProperties(VulkanRenderer::m_VulkanData.m_physicalDevice, &deviceProperties);
		}


		if (VulkanRenderer::m_VulkanData.m_physicalDevice == VK_NULL_HANDLE)
			log("Failed to select GPU!", MESSAGE_TYPE::ERROR);
		else
		{
			std::stringstream ss;
			ss << deviceProperties.deviceName << " is selected!";
			log(ss.str(), MESSAGE_TYPE::INFO);
		}
	}

	void VulkanRenderer::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(VulkanRenderer::m_VulkanData.m_physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(VulkanRenderer::m_VulkanData.m_physicalDevice, &createInfo, nullptr, &VulkanRenderer::m_VulkanData.m_device) != VK_SUCCESS)
			log("Failed to create logical device!", MESSAGE_TYPE::ERROR);
		else
			log("Logical device created successfully", MESSAGE_TYPE::INFO);

		vkGetDeviceQueue(VulkanRenderer::m_VulkanData.m_device, indices.graphicsFamily.value(), 0, &VulkanRenderer::m_VulkanData.m_graphicsQueue);
		vkGetDeviceQueue(VulkanRenderer::m_VulkanData.m_device, indices.presentFamily.value(), 0, &VulkanRenderer::m_VulkanData.m_presentQueue);
	}

	void VulkanRenderer::createSurface()
	{
		if (glfwCreateWindowSurface(VulkanRenderer::m_VulkanData.m_instance, VulkanRenderer::m_VulkanData.m_window, nullptr, &VulkanRenderer::m_VulkanData.m_surface) != VK_SUCCESS)
			log("Failed to create window surface!", MESSAGE_TYPE::ERROR);
		else
			log("Window surface created successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(VulkanRenderer::m_VulkanData.m_physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = VulkanRenderer::m_VulkanData.m_surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(VulkanRenderer::m_VulkanData.m_physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(VulkanRenderer::m_VulkanData.m_device, &createInfo, nullptr, &VulkanRenderer::m_VulkanData.m_swapChain) != VK_SUCCESS)
			log("Failed to create swap chain!", MESSAGE_TYPE::ERROR);
		else
			log("Swap chain created successfully", MESSAGE_TYPE::INFO);

		vkGetSwapchainImagesKHR(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_swapChain, &imageCount, nullptr);
		VulkanRenderer::m_VulkanData.m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_swapChain, &imageCount, VulkanRenderer::m_VulkanData.m_swapChainImages.data());

		VulkanRenderer::m_VulkanData.m_swapChainImageFormat = surfaceFormat.format;
		VulkanRenderer::m_VulkanData.m_swapChainExtent = extent;
	}

	void VulkanRenderer::createImageViews()
	{
		VulkanRenderer::m_VulkanData.m_swapChainImageViews.resize(VulkanRenderer::m_VulkanData.m_swapChainImages.size());

		for (size_t i = 0; i < VulkanRenderer::m_VulkanData.m_swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = VulkanRenderer::m_VulkanData.m_swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = VulkanRenderer::m_VulkanData.m_swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(VulkanRenderer::m_VulkanData.m_device, &createInfo, nullptr, &VulkanRenderer::m_VulkanData.m_swapChainImageViews[i]) != VK_SUCCESS)
				log("Failed to create image views!", MESSAGE_TYPE::ERROR);
		}

		log("Image views created successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = VulkanRenderer::m_VulkanData.m_swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(VulkanRenderer::m_VulkanData.m_device, &renderPassInfo, nullptr, &VulkanRenderer::m_VulkanData.m_renderPass) != VK_SUCCESS)
			log("Failed to create render pass!", MESSAGE_TYPE::ERROR);
		else
			log("Render pass created successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::createGraphicsPipeline()
	{
		auto vertShaderCode = readFile("Shaders/simple_shader.vert.spv");
		auto fragShaderCode = readFile("Shaders/simple_shader.frag.spv");

		VulkanRenderer::m_VulkanData.m_vertShaderModule = createShaderModule(vertShaderCode);
		VulkanRenderer::m_VulkanData.m_fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = VulkanRenderer::m_VulkanData.m_vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = VulkanRenderer::m_VulkanData.m_fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		if (vkCreatePipelineLayout(VulkanRenderer::m_VulkanData.m_device, &pipelineLayoutInfo, nullptr, &VulkanRenderer::m_VulkanData.m_pipelineLayout) != VK_SUCCESS)
			log("Failed to create pipeline layout", MESSAGE_TYPE::ERROR);
		else
			log("Pipeline layout created successfully", MESSAGE_TYPE::INFO);

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = VulkanRenderer::m_VulkanData.m_pipelineLayout;
		pipelineInfo.renderPass = VulkanRenderer::m_VulkanData.m_renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(VulkanRenderer::m_VulkanData.m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &VulkanRenderer::m_VulkanData.m_graphicsPipeline) != VK_SUCCESS)
			log("Failed to create graphics pipeline", MESSAGE_TYPE::ERROR);
		else
			log("Graphics pipeline created successfully", MESSAGE_TYPE::INFO);


		vkDestroyShaderModule(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_fragShaderModule, nullptr);
		vkDestroyShaderModule(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_vertShaderModule, nullptr);
	}

	void VulkanRenderer::createFramebuffers()
	{
		VulkanRenderer::m_VulkanData.m_swapChainFramebuffers.resize(VulkanRenderer::m_VulkanData.m_swapChainImageViews.size());

		for (size_t i = 0; i < VulkanRenderer::m_VulkanData.m_swapChainImageViews.size(); i++) {
			VkImageView attachments[] = {
				VulkanRenderer::m_VulkanData.m_swapChainImageViews[i]
			};
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = VulkanRenderer::m_VulkanData.m_renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = VulkanRenderer::m_VulkanData.m_swapChainExtent.width;
			framebufferInfo.height = VulkanRenderer::m_VulkanData.m_swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(VulkanRenderer::m_VulkanData.m_device, &framebufferInfo, nullptr, &VulkanRenderer::m_VulkanData.m_swapChainFramebuffers[i]) != VK_SUCCESS)
				log("Failed to create framebuffer", MESSAGE_TYPE::ERROR);
		}

		log("Framebuffers created successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(VulkanRenderer::m_VulkanData.m_physicalDevice);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(VulkanRenderer::m_VulkanData.m_device, &poolInfo, nullptr, &VulkanRenderer::m_VulkanData.m_commandPool) != VK_SUCCESS)
			log("Failed to create command pool", MESSAGE_TYPE::ERROR);
		else
			log("Command pool created successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::createVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(VulkanRenderer::m_VulkanData.m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(VulkanRenderer::m_VulkanData.m_device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VulkanRenderer::m_VulkanData.m_vertexBuffer, VulkanRenderer::m_VulkanData.m_vertexBufferMemory);

		copyBuffer(stagingBuffer, VulkanRenderer::m_VulkanData.m_vertexBuffer, bufferSize);

		vkDestroyBuffer(VulkanRenderer::m_VulkanData.m_device, stagingBuffer, nullptr);
		vkFreeMemory(VulkanRenderer::m_VulkanData.m_device, stagingBufferMemory, nullptr);

		log("Vertex buffer memory bound successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::createIndexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(VulkanRenderer::m_VulkanData.m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(VulkanRenderer::m_VulkanData.m_device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VulkanRenderer::m_VulkanData.m_indexBuffer, VulkanRenderer::m_VulkanData.m_indexBufferMemory);

		copyBuffer(stagingBuffer, VulkanRenderer::m_VulkanData.m_indexBuffer, bufferSize);

		vkDestroyBuffer(VulkanRenderer::m_VulkanData.m_device, stagingBuffer, nullptr);
		vkFreeMemory(VulkanRenderer::m_VulkanData.m_device, stagingBufferMemory, nullptr);

		log("Index buffer memory bound successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::createCommandBuffers()
	{
		VulkanRenderer::m_VulkanData.m_commandBuffers.resize(VulkanRenderer::m_VulkanData.m_swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = VulkanRenderer::m_VulkanData.m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)VulkanRenderer::m_VulkanData.m_commandBuffers.size();

		if (vkAllocateCommandBuffers(VulkanRenderer::m_VulkanData.m_device, &allocInfo, VulkanRenderer::m_VulkanData.m_commandBuffers.data()) != VK_SUCCESS)
			log("Failed to allocate command buffers", MESSAGE_TYPE::ERROR);
		else
			log("Command buffers allocated successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = VulkanRenderer::m_VulkanData.m_renderPass;
		renderPassInfo.framebuffer = VulkanRenderer::m_VulkanData.m_swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VulkanRenderer::m_VulkanData.m_swapChainExtent;

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanRenderer::m_VulkanData.m_graphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)VulkanRenderer::m_VulkanData.m_swapChainExtent.width;
		viewport.height = (float)VulkanRenderer::m_VulkanData.m_swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VulkanRenderer::m_VulkanData.m_swapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		VkBuffer vertexBuffers[] = { VulkanRenderer::m_VulkanData.m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, VulkanRenderer::m_VulkanData.m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

		//if (VulkanRenderer::m_VulkanData.m_wireframe)
			//vkCmdSetLineWidth(commandBuffer, 2.0f);

		if (indices.size() == 0)
			vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
		else
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			log("Failed to record command buffer", MESSAGE_TYPE::ERROR);
	}

	uint32_t VulkanRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanRenderer::m_VulkanData.m_physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;

		log("Failed to find suitable memory type", MESSAGE_TYPE::ERROR);

		return 0;
	}

	void VulkanRenderer::createSyncObjects()
	{
		VulkanRenderer::m_VulkanData.m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		VulkanRenderer::m_VulkanData.m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		VulkanRenderer::m_VulkanData.m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		VulkanRenderer::m_VulkanData.m_imagesInFlight.resize(VulkanRenderer::m_VulkanData.m_swapChainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(VulkanRenderer::m_VulkanData.m_device, &semaphoreInfo, nullptr, &VulkanRenderer::m_VulkanData.m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(VulkanRenderer::m_VulkanData.m_device, &semaphoreInfo, nullptr, &VulkanRenderer::m_VulkanData.m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(VulkanRenderer::m_VulkanData.m_device, &fenceInfo, nullptr, &VulkanRenderer::m_VulkanData.m_inFlightFences[i]) != VK_SUCCESS)
				log("failed to create synchronization objects for a frame!", MESSAGE_TYPE::ERROR);
		}

		log("Synchronization objects created successfully", MESSAGE_TYPE::INFO);
	}

	void VulkanRenderer::recreateSwapChain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(VulkanRenderer::m_VulkanData.m_window, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(VulkanRenderer::m_VulkanData.m_window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(VulkanRenderer::m_VulkanData.m_device);

		cleanupSwapChain();

		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandBuffers();
	}

	void VulkanRenderer::cleanupSwapChain()
	{
		for (auto framebuffer : VulkanRenderer::m_VulkanData.m_swapChainFramebuffers) {
			vkDestroyFramebuffer(VulkanRenderer::m_VulkanData.m_device, framebuffer, nullptr);
		}

		vkFreeCommandBuffers(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_commandPool, static_cast<uint32_t>(VulkanRenderer::m_VulkanData.m_commandBuffers.size()), VulkanRenderer::m_VulkanData.m_commandBuffers.data());

		vkDestroyPipeline(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_pipelineLayout, nullptr);
		vkDestroyRenderPass(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_renderPass, nullptr);

		for (size_t i = 0; i < VulkanRenderer::m_VulkanData.m_swapChainImageViews.size(); i++) {
			vkDestroyImageView(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_swapChainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_swapChain, nullptr);
	}

	void VulkanRenderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(VulkanRenderer::m_VulkanData.m_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			log("Failed to create vertex buffer", MESSAGE_TYPE::ERROR);
		else
			log("Vertex buffer created successfully", MESSAGE_TYPE::INFO);

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(VulkanRenderer::m_VulkanData.m_device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(VulkanRenderer::m_VulkanData.m_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
			log("Failed to allocate vertex buffer memory", MESSAGE_TYPE::ERROR);
		else
			log("Vertex buffer memory allocated successfully", MESSAGE_TYPE::INFO);

		vkBindBufferMemory(VulkanRenderer::m_VulkanData.m_device, buffer, bufferMemory, 0);
	}

	void VulkanRenderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = VulkanRenderer::m_VulkanData.m_commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(VulkanRenderer::m_VulkanData.m_device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(VulkanRenderer::m_VulkanData.m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(VulkanRenderer::m_VulkanData.m_graphicsQueue);

		vkFreeCommandBuffers(VulkanRenderer::m_VulkanData.m_device, VulkanRenderer::m_VulkanData.m_commandPool, 1, &commandBuffer);
	}

	bool VulkanRenderer::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	std::vector<const char*> VulkanRenderer::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VulkanRenderer::m_VulkanData.m_surface, &presentSupport);

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

	SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, VulkanRenderer::m_VulkanData.m_surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanRenderer::m_VulkanData.m_surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, VulkanRenderer::m_VulkanData.m_surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanRenderer::m_VulkanData.m_surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, VulkanRenderer::m_VulkanData.m_surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(VulkanRenderer::m_VulkanData.m_window, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(VulkanRenderer::m_VulkanData.m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			log("Failed to create shader module!", MESSAGE_TYPE::ERROR);

		return shaderModule;
	}

	void VulkanRenderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	std::vector<char> VulkanRenderer::readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			std::stringstream ss;
			ss << "[" << RED << "ERROR" << RESET << "] Failed to open file " << filename << "!";
			throw std::runtime_error(ss.str());
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void VulkanRenderer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}

	void VulkanRenderer::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		VulkanRenderer::VulkanRenderer::m_VulkanData.m_framebufferResized = true;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::clog << RESET << "[";
		std::clog << YELLOW << "VALIDATION LAYER" << RESET;
		std::clog << RESET << "] " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanRenderer::log(const std::string& message, MESSAGE_TYPE type)
	{
		std::stringstream ss;

		switch (type)
		{
		case MESSAGE_TYPE::INFO:

			ss << RESET << "[" << GREEN << "INFO" << RESET << "] " << message << RESET;
			std::cout << ss.str() << std::endl;

			break;
		case MESSAGE_TYPE::WARNING:
			ss << RESET << "[" << YELLOW << "WARNING" << RESET << "] " << message << RESET;
			std::cout << ss.str() << std::endl;

			break;
		case MESSAGE_TYPE::ERROR:
			ss << RESET << "[" << RED << "ERROR" << RESET << "] " << message << RESET;
			throw std::runtime_error(ss.str());

			break;
		default:
			break;
		}
	}
}