#pragma once

#include <ShaderGenerator/ShaderGeneratorExport.hpp>
#include <ShaderGenerator/Generators/ShaderRenderInfo.hpp>

#include <string>
#include <vector>

namespace sg
{
	class SG_API BaseShaderGenerator
	{
	protected:
		std::vector<std::string> m_structures;
		std::vector<std::string> m_inputVariables;
		std::vector<std::string> m_outputVariables;
		std::vector<std::string> m_uniformVariables;
		std::vector<std::string> m_functions;

		ShaderRenderInfo m_renderInfo;

		virtual void generateStructures() = 0;
		virtual void generateInputVariables() = 0;
		virtual void generateOutputVariables() = 0;
		virtual void generateUniformVariables() = 0;
		virtual void generateFunctions() = 0;

	public:
		BaseShaderGenerator();

		std::string generate();

		ShaderRenderInfo getRenderInfo() const;
	};
}
