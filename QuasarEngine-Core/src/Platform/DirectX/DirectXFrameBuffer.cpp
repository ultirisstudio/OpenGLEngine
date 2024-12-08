#include "qepch.h"
#include "DirectXFramebuffer.h"

#include <stdexcept>
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace QuasarEngine
{
    DirectXFramebuffer::DirectXFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec) {
        Invalidate();
    }

    void DirectXFramebuffer::Invalidate() {
        
    }

    void DirectXFramebuffer::Resize(uint32_t width, uint32_t height) {
        
    }

    uint32_t DirectXFramebuffer::GetColorAttachment(uint32_t index) const {
        return 0;
    }

    uint32_t DirectXFramebuffer::GetDepthAttachment() const {
        return 0;
    }

    uint32_t DirectXFramebuffer::GetID() const {
        return 0;
    }

    const FramebufferSpecification& DirectXFramebuffer::getSpecification() const {
        return m_Specification;
    }

    int DirectXFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
        
        return 0;
    }

    void DirectXFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
        
    }

    void DirectXFramebuffer::Bind() const {
        
    }

    void DirectXFramebuffer::Unbind() const {
        
    }

    void DirectXFramebuffer::BindColorAttachment(uint32_t index) const {
        
    }
}