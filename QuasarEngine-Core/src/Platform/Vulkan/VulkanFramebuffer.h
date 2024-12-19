#pragma once

#include "VulkanTypes.h"

#include <QuasarEngine/Renderer/Framebuffer.h>

namespace QuasarEngine
{
	class VulkanFramebuffer : public Framebuffer
	{
    private:
        struct vulkanFramebuffer
        {
            VkFramebuffer handle;
            std::vector<VkImageView> attachments;
            VkRenderPass* renderPass;
        };
        vulkanFramebuffer m_framebuffer;
    public:
        VulkanFramebuffer(const FramebufferSpecification& spec);
        ~VulkanFramebuffer();

        uint32_t GetColorAttachment(uint32_t index) const override;
        uint32_t GetDepthAttachment() const override;

        uint32_t GetID() const override;

        const FramebufferSpecification& getSpecification() const override;

        int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        void ClearAttachment(uint32_t attachmentIndex, int value) override;

        void Resize(uint32_t width, uint32_t height) override;
        void Invalidate() override;

        void Bind() const override;
        void Unbind() const override;

        void BindColorAttachment(uint32_t index = 0) const override;
    private:
        FramebufferSpecification m_Specification;
	};
}