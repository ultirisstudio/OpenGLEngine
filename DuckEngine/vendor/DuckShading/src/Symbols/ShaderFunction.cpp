#include <ShaderGenerator/Symbols/ShaderFunction.hpp>

#include <sstream>

namespace sg
{
	ShaderFunction::ShaderFunction(const std::string& name, const std::string type) :
		m_name(name),
		m_type(type),
		m_instructions()
	{

	}

	void ShaderFunction::addInstruction(const std::string instruction)
	{
		m_instructions.push_back(instruction);
	}

	std::string ShaderFunction::generate() const
	{
		std::ostringstream stream;

		stream << m_type << " " << m_name << "()\n{\n";

		for (std::string instruction : m_instructions)
			stream << "\t" << instruction << "\n";

		stream << "};\n";

		return stream.str();
	}
}
