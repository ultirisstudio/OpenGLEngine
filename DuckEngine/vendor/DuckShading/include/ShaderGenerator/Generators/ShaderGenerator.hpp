#pragma once

#include <ShaderGenerator/Generators/BPhongVertexShaderGenerator.hpp>
#include <ShaderGenerator/Generators/BPhongFragmentShaderGenerator.hpp>
#include <ShaderGenerator/Generators/ShaderType.hpp>
#include <ShaderGenerator/Materials/Material.hpp>

namespace sg
{
	class SG_API ShaderGenerator
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
