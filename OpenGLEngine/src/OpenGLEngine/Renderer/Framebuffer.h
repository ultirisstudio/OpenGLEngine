#pragma once

#include <vector>

namespace OpenGLEngine
{
    class Framebuffer
    {
    private:
        unsigned int m_id;

        std::vector<unsigned int> m_colorAttachments;
        unsigned int m_depthAttachment;
        unsigned int m_stencilAttachment;

        unsigned int m_width;
        unsigned int m_height;

    public:
        Framebuffer(unsigned int width, unsigned int height);
        ~Framebuffer();

        unsigned int getColorAttachment(unsigned int index) const;
        unsigned int getDepthAttachment() const;
        unsigned int getStencilAttachment() const;

        unsigned int getId() const;

        void bind() const;
        void unbind() const;

        void addColorAttachment(unsigned int internalFormat, unsigned int format, unsigned int type);
        void setDepthAttachment();
        void setStencilAttachment();

        void Create();
    };
}
