#include <ShaderGenerator/Generators/BaseShaderGenerator.hpp>

#include <sstream>

namespace sg
{
	BaseShaderGenerator::BaseShaderGenerator() :
		m_structures(),
		m_inputVariables(),
		m_outputVariables(),
		m_uniformVariables(),
		m_functions(),
		m_renderInfo()
	{

	}

	std::string BaseShaderGenerator::generate()
	{
		generateStructures();
		generateInputVariables();
		generateOutputVariables();
		generateUniformVariables();
		generateFunctions();

		std::ostringstream stream;

		stream << "#version 330 core\n\n";

		for (std::string structure : m_structures)
			stream << structure << "\n";

		for (std::string inputVariable : m_inputVariables)
			stream << inputVariable;

		if (!m_inputVariables.empty())
			stream << "\n";

		for (std::string outputVariable : m_outputVariables)
			stream << outputVariable;

		if (!m_outputVariables.empty())
			stream << "\n";

		for (std::string uniformVariable : m_uniformVariables)
			stream << uniformVariable;

		if (!m_uniformVariables.empty())
			stream << "\n";

		for (std::string function : m_functions)
			stream << function << "\n";

		return stream.str();
	}

	ShaderRenderInfo BaseShaderGenerator::getRenderInfo() const
	{
		return m_renderInfo;
	}
}
