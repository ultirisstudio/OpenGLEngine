#include "qepch.h"
#include "VulkanFramebuffer.h"
#include "VulkanContext.h"
#include "VulkanRenderPass.h"

namespace QuasarEngine
{
    namespace Utils
    {
        static bool isDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                return true;
            default:
                return false;
            }
        }
    }

    VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec) {
        if (!spec.Attachments.Attachments.empty())
        {
            for (auto format : spec.Attachments.Attachments)
            {
                if (!Utils::isDepthFormat(format.TextureFormat))
                {
                    //m_framebuffer.attachments.push_back();
                }
            }

            Invalidate();
        }
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {

    }

    void VulkanFramebuffer::Invalidate()
    {
        VkFramebufferCreateInfo framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
    }

    void VulkanFramebuffer::Resize(uint32_t width, uint32_t height) {
        
    }

    uint32_t VulkanFramebuffer::GetColorAttachment(uint32_t index) const {
        return 0;
    }

    uint32_t VulkanFramebuffer::GetDepthAttachment() const {
        return 0;
    }

    uint32_t VulkanFramebuffer::GetID() const {
        return 0;
    }

    const FramebufferSpecification& VulkanFramebuffer::getSpecification() const {
        return m_Specification;
    }

    int VulkanFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
        
        return 0;
    }

    void VulkanFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
        
    }

    void VulkanFramebuffer::Bind() const {
        
    }

    void VulkanFramebuffer::Unbind() const {
        
    }

    void VulkanFramebuffer::BindColorAttachment(uint32_t index) const {
        
    }
}