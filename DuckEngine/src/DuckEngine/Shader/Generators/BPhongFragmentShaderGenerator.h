////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DuckEngine/Shader/Generators/BaseShaderGenerator.h>
#include <DuckEngine/Resources/Materials/BPhongMaterial.h>
#include <DuckEngine/Shader/Symbols/ShaderStructure.h>

namespace DuckEngine
{
	class BPhongFragmentShaderGenerator : public BaseShaderGenerator
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
