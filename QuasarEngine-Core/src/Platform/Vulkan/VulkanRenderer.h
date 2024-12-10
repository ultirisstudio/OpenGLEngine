#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace QuasarEngine
{
	enum class MESSAGE_TYPE
	{
		INFO,
		WARNING,
		ERROR
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanRenderer
	{
	public:
		struct VulkanData
		{
			VkInstance m_instance;
			VkDebugUtilsMessengerEXT m_callback;
			VkPhysicalDevice m_physicalDevice;
			VkDevice m_device;
			VkQueue m_graphicsQueue;
			VkSurfaceKHR m_surface;
			VkQueue m_presentQueue;
			VkSwapchainKHR m_swapChain;
			VkFormat m_swapChainImageFormat;
			VkExtent2D m_swapChainExtent;
			VkShaderModule m_vertShaderModule;
			VkShaderModule m_fragShaderModule;
			VkRenderPass m_renderPass;
			VkPipelineLayout m_pipelineLayout;
			VkPipeline m_graphicsPipeline;
			VkCommandPool m_commandPool;

			VkBuffer m_vertexBuffer;
			VkDeviceMemory m_vertexBufferMemory;
			VkBuffer m_indexBuffer;
			VkDeviceMemory m_indexBufferMemory;

			std::vector<VkSemaphore> m_imageAvailableSemaphores;
			std::vector<VkSemaphore> m_renderFinishedSemaphores;
			std::vector<VkFence> m_inFlightFences;
			std::vector<VkFence> m_imagesInFlight;

			std::vector<VkImage> m_swapChainImages;
			std::vector<VkImageView> m_swapChainImageViews;
			std::vector<VkFramebuffer> m_swapChainFramebuffers;
			std::vector<VkCommandBuffer> m_commandBuffers;

			size_t m_currentFrame = 0;
			bool m_framebufferResized = false;

			const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

			const std::vector<const char*> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};
		};
		static VulkanData m_VulkanData;

		static void Init(GLFWwindow* window);

		static void DrawFrame();
	private:
		static void initVulkan();
		static void cleanup();

		static void createInstance();
		static void setupDebugMessenger();
		static void createSurface();
		static void pickPhysicalDevice();
		static void createLogicalDevice();
		static void createSwapChain();
		static void createImageViews();
		static void createRenderPass();
		static void createGraphicsPipeline();
		static void createFramebuffers();
		static void createCommandPool();
		static void createVertexBuffer();
		static void createIndexBuffer();
		static void createCommandBuffers();
		static void createSyncObjects();
	private:
		static void recreateSwapChain();
		static void cleanupSwapChain();

		static void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		static bool checkValidationLayerSupport();
		static bool isDeviceSuitable(VkPhysicalDevice device);
		static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static std::vector<const char*> getRequiredExtensions();

		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		static VkShaderModule createShaderModule(const std::vector<char>& code);

		static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		static std::vector<char> readFile(const std::string& filename);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		static VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pCallback);

		static void DestroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT callback,
			const VkAllocationCallbacks* pAllocator);

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	private:
		void log(const std::string& message, MESSAGE_TYPE type);
	};
}