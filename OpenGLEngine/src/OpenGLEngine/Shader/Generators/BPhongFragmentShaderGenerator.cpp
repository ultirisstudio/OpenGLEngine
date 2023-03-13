////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "depch.h"

#include <OpenGLEngine/Shader/Generators/BPhongFragmentShaderGenerator.h>
#include <OpenGLEngine/Shader/Symbols/Loader.h>
#include <OpenGLEngine/Shader/Symbols/ShaderFunction.h>
#include <OpenGLEngine/Shader/Symbols/ShaderVariable.h>

namespace OpenGLEngine
{ 
	void BPhongFragmentShaderGenerator::generateStructures()
	{
		m_structures.push_back(m_vertexDataStruct.generate());
		m_structures.push_back(m_materialStruct.generate());
		m_structures.push_back(Loader::readPiece("res/structures/light_material.glsl"));
		m_structures.push_back(Loader::readPiece("res/structures/point_light.glsl"));
	}

	void BPhongFragmentShaderGenerator::generateInputVariables()
	{
		m_inputVariables.push_back(ShaderVariable("fPosition", "vec3", ShaderVariableQualifier::T_IN).generate());
		m_inputVariables.push_back(ShaderVariable("fNormal", "vec3", ShaderVariableQualifier::T_IN).generate());

		if (m_material.hasAnyTexture)
			m_inputVariables.push_back(ShaderVariable("fTextureCoordinates", "vec2", ShaderVariableQualifier::T_IN).generate());
	}

	void BPhongFragmentShaderGenerator::generateOutputVariables()
	{
		m_outputVariables.push_back(ShaderVariable("color", "vec4", ShaderVariableQualifier::T_OUT).generate());
	}

	void BPhongFragmentShaderGenerator::generateUniformVariables()
	{
		m_uniformVariables.push_back(ShaderVariable("uMaterial", "Material", ShaderVariableQualifier::UNIFORM).generate());
		m_uniformVariables.push_back(ShaderVariable("uPointLight", "PointLight", ShaderVariableQualifier::UNIFORM).generate());
		m_uniformVariables.push_back(ShaderVariable("uViewPos", "vec3", ShaderVariableQualifier::UNIFORM).generate());

		m_renderInfo.addRenderInfo("uMaterial.ambient", m_material.ambient.getType());
		m_renderInfo.addRenderInfo("uMaterial.diffuse", m_material.diffuse.getType());
		m_renderInfo.addRenderInfo("uMaterial.specular", m_material.specular.getType());
		m_renderInfo.addRenderInfo("uMaterial.shininess", m_material.shininess.getType());
		m_renderInfo.addRenderInfo("uPointLight.ambient", "vec3");
		m_renderInfo.addRenderInfo("uPointLight.diffuse", "vec3");
		m_renderInfo.addRenderInfo("uPointLight.specular", "vec3");
		m_renderInfo.addRenderInfo("uPointLight.position", "vec3");
		m_renderInfo.addRenderInfo("uPointLight.constant", "float");
		m_renderInfo.addRenderInfo("uPointLight.linear", "float");
		m_renderInfo.addRenderInfo("uPointLight.quadratic", "float");
		m_renderInfo.addRenderInfo("uViewPos", "vec3");
	}

	void BPhongFragmentShaderGenerator::generateFunctions()
	{
		m_functions.push_back(Loader::readPiece("res/functions/calculate_point_light.glsl"));

		ShaderFunction mainFunction("main", "void");

		// Conversion du Material en LightMaterial

		mainFunction.addInstruction("LightMaterial lightMaterial;");

		if (m_material.ambient.isTexture())
			mainFunction.addInstruction("lightMaterial.ambient = texture(uMaterial.ambient, fTextureCoordinates).xyz;");
		else
			mainFunction.addInstruction("lightMaterial.ambient = uMaterial.ambient;");

		if (m_material.diffuse.isTexture())
			mainFunction.addInstruction("lightMaterial.diffuse = texture(uMaterial.diffuse, fTextureCoordinates).xyz;");
		else
			mainFunction.addInstruction("lightMaterial.diffuse = uMaterial.diffuse;");

		if (m_material.specular.isTexture())
			mainFunction.addInstruction("lightMaterial.specular = texture(uMaterial.specular, fTextureCoordinates).xyz;");
		else
			mainFunction.addInstruction("lightMaterial.specular = uMaterial.specular;");

		mainFunction.addInstruction("lightMaterial.shininess = uMaterial.shininess;");
		mainFunction.addInstruction("");

		// Cr�ation de l'instance de la structure VertexData

		mainFunction.addInstruction("VertexData vertex;");
		mainFunction.addInstruction("vertex.position = fPosition;");
		mainFunction.addInstruction("vertex.normal = fNormal;");
		mainFunction.addInstruction("");

		mainFunction.addInstruction("vec3 viewDir = normalize(uViewPos - vertex.position);");
		mainFunction.addInstruction("color = vec4(calculatePointLight(uPointLight, lightMaterial, vertex, viewDir), 1.0f);");

		m_functions.push_back(mainFunction.generate());
	}

	BPhongFragmentShaderGenerator::BPhongFragmentShaderGenerator(const BPhongMaterial& material) :
		BaseShaderGenerator(),
		m_material(material),
		m_materialStruct("Material"),
		m_vertexDataStruct("VertexData")
	{
		m_materialStruct.addAttribute(m_material.ambient.getType(), "ambient");
		m_materialStruct.addAttribute(m_material.diffuse.getType(), "diffuse");
		m_materialStruct.addAttribute(m_material.specular.getType(), "specular");
		m_materialStruct.addAttribute(m_material.shininess.getType(), "shininess");

		m_vertexDataStruct.addAttribute("vec3", "position");
		m_vertexDataStruct.addAttribute("vec3", "normal");
	}
}
