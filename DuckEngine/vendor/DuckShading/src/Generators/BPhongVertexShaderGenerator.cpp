#include <ShaderGenerator/Generators/BPhongVertexShaderGenerator.hpp>
#include <ShaderGenerator/Symbols/ShaderFunction.hpp>
#include <ShaderGenerator/Symbols/ShaderVariable.hpp>

namespace sg
{
	void BPhongVertexShaderGenerator::generateStructures()
	{

	}

	void BPhongVertexShaderGenerator::generateInputVariables()
	{
		m_inputVariables.push_back(ShaderVariable("vPosition", "vec3", ShaderVariableQualifier::IN, 0).generate());
		m_inputVariables.push_back(ShaderVariable("vNormal", "vec3", ShaderVariableQualifier::IN, 1).generate());

		if(m_material.hasAnyTexture)
			m_inputVariables.push_back(ShaderVariable("vTextureCoordinates", "vec2", ShaderVariableQualifier::IN, 2).generate());
	}

	void BPhongVertexShaderGenerator::generateOutputVariables()
	{
		m_outputVariables.push_back(ShaderVariable("fPosition", "vec3", ShaderVariableQualifier::OUT).generate());
		m_outputVariables.push_back(ShaderVariable("fNormal", "vec3", ShaderVariableQualifier::OUT).generate());

		if (m_material.hasAnyTexture)
			m_outputVariables.push_back(ShaderVariable("fTextureCoordinates", "vec2", ShaderVariableQualifier::OUT).generate());
	}

	void BPhongVertexShaderGenerator::generateUniformVariables()
	{
		m_uniformVariables.push_back(ShaderVariable("uModel", "mat4", ShaderVariableQualifier::UNIFORM).generate());
		m_uniformVariables.push_back(ShaderVariable("uView", "mat4", ShaderVariableQualifier::UNIFORM).generate());
		m_uniformVariables.push_back(ShaderVariable("uProjection", "mat4", ShaderVariableQualifier::UNIFORM).generate());
	
		m_renderInfo.addRenderInfo("uModel", "mat4");
		m_renderInfo.addRenderInfo("uView", "mat4");
		m_renderInfo.addRenderInfo("uProjection", "mat4");
	}

	void BPhongVertexShaderGenerator::generateFunctions()
	{
		ShaderFunction mainFunction("main", "void");

		mainFunction.addInstruction("vec4 worldPosition = uModel * vec4(vPosition, 1.0f);");
		mainFunction.addInstruction("");

		mainFunction.addInstruction("fPosition = worldPosition.xyz;");
		mainFunction.addInstruction("fNormal = mat3(transpose(inverse(uModel))) * vNormal;");

		if (m_material.hasAnyTexture)
			mainFunction.addInstruction("fTextureCoordinates = vTextureCoordinates;");

		mainFunction.addInstruction("");
		mainFunction.addInstruction("gl_Position = uProjection * uView * worldPosition;");

		m_functions.push_back(mainFunction.generate());
	}

	BPhongVertexShaderGenerator::BPhongVertexShaderGenerator(const BPhongMaterial& material) :
		BaseShaderGenerator(),
		m_material(material)
	{

	}
}
