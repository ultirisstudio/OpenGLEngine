////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <OpenGLEngine/Shader/Generators/BPhongVertexShaderGenerator.h>
#include <OpenGLEngine/Shader/Generators/BPhongFragmentShaderGenerator.h>
#include <OpenGLEngine/Shader/Generators/ShaderType.h>
#include <OpenGLEngine/Resources/Materials/Material.h>

namespace OpenGLEngine
{
	class ShaderGenerator
	{
	private:
		BaseShaderGenerator* m_vertexShaderGenerator;
		BaseShaderGenerator* m_fragmentShaderGenerator;

	public:
		ShaderGenerator(const Material& material, ShaderType shaderType);
		~ShaderGenerator();

		std::string generateVertexShader();
		std::string generateFragmentShader();

		ShaderRenderInfo getVertexShaderRenderInfo() const;
		ShaderRenderInfo getFragmentShaderRenderInfo() const;
	};
}
