////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <OpenGLEngine/Shader/Generators/BaseShaderGenerator.h>
#include <OpenGLEngine/Resources/Materials/BPhongMaterial.h>
#include <OpenGLEngine/Shader/Symbols/ShaderStructure.h>

namespace OpenGLEngine
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
