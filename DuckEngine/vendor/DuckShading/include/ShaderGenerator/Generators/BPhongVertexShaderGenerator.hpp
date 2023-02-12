#pragma once

#include <ShaderGenerator/Generators/BaseShaderGenerator.hpp>
#include <ShaderGenerator/Materials/BPhongMaterial.hpp>

namespace sg
{
	class SG_API BPhongVertexShaderGenerator : public BaseShaderGenerator
	{
	private:
		BPhongMaterial m_material;

		void generateStructures() override;
		void generateInputVariables() override;
		void generateOutputVariables() override;
		void generateUniformVariables() override;
		void generateFunctions() override;

	public:
		BPhongVertexShaderGenerator(const BPhongMaterial& material);
	};
}
