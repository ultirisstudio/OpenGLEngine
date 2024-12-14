#include "qepch.h"
#include "VulkanFramebuffer.h"

namespace QuasarEngine
{
    VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec) {
        Invalidate();
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {

    }

    void VulkanFramebuffer::Invalidate() {
        
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