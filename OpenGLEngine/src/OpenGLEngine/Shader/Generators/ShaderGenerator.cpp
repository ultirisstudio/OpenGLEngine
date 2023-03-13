////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "depch.h"

#include <OpenGLEngine/Shader/Generators/ShaderGenerator.h>
#include <OpenGLEngine/Shader/Materials/MaterialStructGenerator.h>

namespace OpenGLEngine
{
    ShaderGenerator::ShaderGenerator(const Material& material, ShaderType shaderType) :
        m_vertexShaderGenerator(nullptr),
        m_fragmentShaderGenerator(nullptr)
    {
        switch (shaderType)
        {
        case ShaderType::BPhong:
            BPhongMaterial bphongMaterial = MaterialStructGenerator::generateBPhongMaterial(material);
            m_vertexShaderGenerator = new BPhongVertexShaderGenerator(bphongMaterial);
            m_fragmentShaderGenerator = new BPhongFragmentShaderGenerator(bphongMaterial);
            break;
        }
    }

    ShaderGenerator::~ShaderGenerator()
    {
        if(m_vertexShaderGenerator)
            delete m_vertexShaderGenerator;

        if (m_fragmentShaderGenerator)
            delete m_fragmentShaderGenerator;
    }

    std::string ShaderGenerator::generateVertexShader()
    {
        return m_vertexShaderGenerator->generate();
    }

    std::string ShaderGenerator::generateFragmentShader()
    {
        return m_fragmentShaderGenerator->generate();
    }

    ShaderRenderInfo ShaderGenerator::getVertexShaderRenderInfo() const
    {
        return m_vertexShaderGenerator->getRenderInfo();
    }

    ShaderRenderInfo ShaderGenerator::getFragmentShaderRenderInfo() const
    {
        return m_fragmentShaderGenerator->getRenderInfo();
    }
}
