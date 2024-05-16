#pragma once

namespace OpenGLEngine
{
    class ScreenQuad
    {
    private:
        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_ebo;

    public:
        ScreenQuad();
        ~ScreenQuad();

        void Draw() const;
    };
}