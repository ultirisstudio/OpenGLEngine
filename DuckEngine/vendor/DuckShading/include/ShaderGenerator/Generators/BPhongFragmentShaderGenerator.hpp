#pragma once

#include <ShaderGenerator/Generators/BaseShaderGenerator.hpp>
#include <ShaderGenerator/Materials/BPhongMaterial.hpp>
#include <ShaderGenerator/Symbols/ShaderStructure.hpp>

namespace sg
{
	class SG_API BPhongFragmentShaderGenerator : public BaseShaderGenerator
	{
	private:
		BPhongMaterial m_material;
		ShaderStructure m_materialStruct;
		ShaderStructure m_vertexDataStruct;

		void generateStructures() override;
		void generateInputVariables() override;
		void generateOutputVariables() override;
		void generateUniformVariables() override;
		void generateFunctions() override;

	public:
		BPhongFragmentShaderGenerator(const BPhongMaterial& material);
	};
}
