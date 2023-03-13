#include "depch.h"
#include "Framebuffer.h"
#include <glad/glad.h>

namespace OpenGLEngine
{
    Framebuffer::Framebuffer(unsigned int width, unsigned int height) :
        m_id(0),
        m_colorAttachments(),
        m_depthAttachment(0),
        m_stencilAttachment(0),
        m_width(width),
        m_height(height)
    {

    }

    Framebuffer::~Framebuffer()
    {
        glDeleteTextures((GLsizei)m_colorAttachments.size(), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);
        glDeleteTextures(1, &m_stencilAttachment);
        glDeleteFramebuffers(1, &m_id);
    }

    unsigned int Framebuffer::getColorAttachment(unsigned int index) const
    {
        return m_colorAttachments[index];
    }

    unsigned int Framebuffer::getDepthAttachment() const
    {
        return m_depthAttachment;
    }

    unsigned int Framebuffer::getStencilAttachment() const
    {
        return m_stencilAttachment;
    }

    unsigned int Framebuffer::getId() const
    {
        return m_id;
    }

    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::addColorAttachment(unsigned int internalFormat, unsigned int format, unsigned int type)
    {
        unsigned int attachment;

        glGenTextures(1, &attachment);
        glBindTexture(GL_TEXTURE_2D, attachment);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, type, NULL);

        m_colorAttachments.push_back(attachment);
    }

    void Framebuffer::setDepthAttachment()
    {
        unsigned int attachment;

        glGenTextures(1, &attachment);
        glBindTexture(GL_TEXTURE_2D, attachment);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        m_depthAttachment = attachment;
    }

    void Framebuffer::setStencilAttachment()
    {
        unsigned int attachment;

        glGenTextures(1, &attachment);
        glBindTexture(GL_TEXTURE_2D, attachment);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, m_width, m_height, 0, GL_STENCIL_INDEX, GL_FLOAT, NULL);

        m_stencilAttachment = attachment;
    }

    void Framebuffer::Create()
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        for (unsigned int i = 0; i < m_colorAttachments.size(); i++)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorAttachments[i], 0);
        if (m_depthAttachment != 0)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);
        if (m_stencilAttachment != 0)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilAttachment, 0);

        std::vector<unsigned int> colorAttachments;
        for (unsigned int i = 0; i < m_colorAttachments.size(); i++)
            colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);

        glDrawBuffers((GLsizei)colorAttachments.size(), colorAttachments.data());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "framebuffer incomplete" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}