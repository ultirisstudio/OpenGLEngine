#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>
#include <memory>

#include <QuasarEngine/Core/Log.h>

namespace QuasarEngine
{
	class VulkanSwapchain;
	class VulkanDevice;
	class VulkanImage;
	class VulkanRenderPass;
	class VulkanCommandBuffer;

	enum vulkanCommandBufferState : uint8_t
	{
		COMMAND_BUFFER_STATE_READY,
		COMMAND_BUFFER_STATE_RECORDING,
		COMMAND_BUFFER_STATE_IN_RENDER_PASS,
		COMMAND_BUFFER_STATE_RECORDING_ENDED,
		COMMAND_BUFFER_STATE_SUBMITTED,
		COMMAND_BUFFER_STATE_NOT_ALLOCATED
	};

	struct vulkanCommandBuffer
	{
		VkCommandBuffer handle;

		vulkanCommandBufferState state;
	};

	enum vulkanRenderPassState : uint8_t
	{
		READY,
		RECORDING,
		IN_RENDER_PASS,
		RECORDING_ENDED,
		SUBMITTED,
		NOT_ALLOCATED
	};

	struct vulkanRenderPass
	{
		VkRenderPass handle;

		float x, y, w, h;
		float r, g, b, a;

		float depth;
		uint32_t stencil;

		vulkanRenderPassState state;
	};

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
		std::unique_ptr<VulkanRenderPass> mainRenderPass;

		std::vector<std::unique_ptr<VulkanCommandBuffer>> graphicsCommandBuffer;

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