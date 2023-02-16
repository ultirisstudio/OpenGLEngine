////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DuckEngine/Shader/Generators/BaseShaderGenerator.h>
#include <DuckEngine/Resources/Materials/BPhongMaterial.h>

namespace DuckEngine
{
	class BPhongVertexShaderGenerator : public BaseShaderGenerator
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
