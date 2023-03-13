////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <OpenGLEngine/Shader/Generators/BaseShaderGenerator.h>
#include <OpenGLEngine/Resources/Materials/BPhongMaterial.h>

namespace OpenGLEngine
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
