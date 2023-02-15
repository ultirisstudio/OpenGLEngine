////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DuckEngine/Shader/Generators/BPhongVertexShaderGenerator.h>
#include <DuckEngine/Shader/Generators/BPhongFragmentShaderGenerator.h>
#include <DuckEngine/Shader/Generators/ShaderType.h>
#include <DuckEngine/Shader/Materials/Material.h>

namespace DuckEngine
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
