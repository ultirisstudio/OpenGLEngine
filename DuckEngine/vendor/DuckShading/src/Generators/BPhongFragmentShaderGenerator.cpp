#include <ShaderGenerator/Generators/BPhongFragmentShaderGenerator.hpp>
#include <ShaderGenerator/Symbols/Loader.hpp>
#include <ShaderGenerator/Symbols/ShaderFunction.hpp>
#include <ShaderGenerator/Symbols/ShaderVariable.hpp>

namespace sg
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
		m_inputVariables.push_back(ShaderVariable("fPosition", "vec3", ShaderVariableQualifier::IN).generate());
		m_inputVariables.push_back(ShaderVariable("fNormal", "vec3", ShaderVariableQualifier::IN).generate());

		if (m_material.hasAnyTexture)
			m_inputVariables.push_back(ShaderVariable("vTextureCoordinates", "vec2", ShaderVariableQualifier::IN).generate());
	}

	void BPhongFragmentShaderGenerator::generateOutputVariables()
	{
		m_outputVariables.push_back(ShaderVariable("color", "vec4", ShaderVariableQualifier::OUT).generate());
	}

	void BPhongFragmentShaderGenerator::generateUniformVariables()
	{
		m_uniformVariables.push_back(ShaderVariable("uMaterial", "Material", ShaderVariableQualifier::UNIFORM).generate());
		m_uniformVariables.push_back(ShaderVariable("uPointLight", "PointLight", ShaderVariableQualifier::UNIFORM).generate());
		m_uniformVariables.push_back(ShaderVariable("uViewPos", "vec3", ShaderVariableQualifier::UNIFORM).generate());
	
		m_renderInfo.addRenderInfo("uMaterial", "Material");
		m_renderInfo.addRenderInfo("uPointLight", "PointLight");
		m_renderInfo.addRenderInfo("uViewPos", "vec3");
	}

	void BPhongFragmentShaderGenerator::generateFunctions()
	{
		m_functions.push_back(Loader::readPiece("res/functions/calculate_point_light.glsl"));

		ShaderFunction mainFunction("main", "void");

		// Conversion du Material en LightMaterial

		mainFunction.addInstruction("LightMaterial lightMaterial;");
		mainFunction.addInstruction("lightMaterial.ambient = uMaterial.ambient;");

		if (m_material.diffuse.isTexture())
			mainFunction.addInstruction("lightMaterial.diffuse = texture(uMaterial.diffuse, fTextureCoordinates);");
		else
			mainFunction.addInstruction("lightMaterial.diffuse = uMaterial.diffuse;");

		if (m_material.specular.isTexture())
			mainFunction.addInstruction("lightMaterial.specular = texture(uMaterial.specular, fTextureCoordinates);");
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
		mainFunction.addInstruction("vec4 color = vec4(calculatePointLight(uPointLight, lightMaterial, vertex, viewDir), 1.0f);");

		m_functions.push_back(mainFunction.generate());
	}

	BPhongFragmentShaderGenerator::BPhongFragmentShaderGenerator(const BPhongMaterial& material) :
		BaseShaderGenerator(),
		m_material(material),
		m_materialStruct("Material"),
		m_vertexDataStruct("VertexData")
	{
		m_materialStruct.addAttribute(m_material.ambient.getType(), "ambient");
		m_materialStruct.addAttribute(m_material.ambient.getType(), "diffuse");
		m_materialStruct.addAttribute(m_material.ambient.getType(), "specular");
		m_materialStruct.addAttribute(m_material.ambient.getType(), "shininess");

		m_vertexDataStruct.addAttribute("vec3", "position");
		m_vertexDataStruct.addAttribute("vec3", "normal");
	}
}
