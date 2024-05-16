#pragma once

#include <vector>
#include <memory>

namespace OpenGLEngine
{
    enum class FramebufferTextureFormat
    {
		None = 0,

		RGBA8,
		RED_INTEGER,

		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

    struct FramebufferTextureSpecification
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}
        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
    };

    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : Attachments(attachments) {}
        std::vector<FramebufferTextureSpecification> Attachments;
    };

    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        FramebufferAttachmentSpecification Attachments;
        uint32_t Samples = 1;
    };

    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();

        uint32_t GetColorAttachment(uint32_t index) const;
        uint32_t GetDepthAttachment() const;

        uint32_t GetID() const;

        const FramebufferSpecification& getSpecification() const { return m_Specification; }

        int ReadPixel(uint32_t attachmentIndex, int x, int y);

        void ClearAttachment(uint32_t attachmentIndex, int value);

        void Resize(uint32_t width, uint32_t height);
        void Invalidate();

        void Bind() const;
        void Unbind() const;

        void BindColorAttachment(uint32_t index = 0) const;

        static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
    private:
        uint32_t m_ID;
        FramebufferSpecification m_Specification;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}
