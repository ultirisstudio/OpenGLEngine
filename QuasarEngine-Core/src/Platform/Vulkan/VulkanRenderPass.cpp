#include "qepch.h"
#include "VulkanRenderPass.h"

#include "VulkanContext.h"
#include "VulkanSwapchain.h"
#include "VulkanDevice.h"

namespace QuasarEngine
{
	void VulkanRenderPass::CreateRenderPass(float x, float y, float w, float h, float r, float g, float b, float a, float depth, uint32_t stencil)
	{
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		const uint32_t attachment_descriptor_count = 2;
		VkAttachmentDescription attachment_descriptions[attachment_descriptor_count];

		VkAttachmentDescription color_attachment;
		color_attachment.format = VulkanContext::m_VulkanContext.swapchain->GetSwapchain().imageFormat.format;
		color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		color_attachment.flags = 0;

		attachment_descriptions[0] = color_attachment;

		VkAttachmentReference color_attachment_reference;
		color_attachment_reference.attachment = 0;
		color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment_reference;

		VkAttachmentDescription depth_attachment;
		depth_attachment.format = VulkanContext::m_VulkanContext.device->GetDevice().depthFormat;
		depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depth_attachment.flags = 0;

		attachment_descriptions[1] = depth_attachment;

		VkAttachmentReference depth_attachment_reference;
		depth_attachment_reference.attachment = 1;
		depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		subpass.pDepthStencilAttachment = &depth_attachment_reference;

		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = 0;

		subpass.pResolveAttachments = 0;

		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = 0;

		VkSubpassDependency dependency;
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dependencyFlags = 0;

		VkRenderPassCreateInfo createInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		createInfo.attachmentCount = attachment_descriptor_count;
		createInfo.pAttachments = attachment_descriptions;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;
		createInfo.pNext = 0;
		createInfo.flags = 0;

		VK_CHECK(vkCreateRenderPass(VulkanContext::m_VulkanContext.device->GetDevice().logicalDevice, &createInfo, VulkanContext::m_VulkanContext.allocator, m_renderPass.handle));
	}

	void VulkanRenderPass::DestroyRenderPass()
	{
	}

	void VulkanRenderPass::Begin()
	{
	}

	void VulkanRenderPass::End()
	{
	}
}