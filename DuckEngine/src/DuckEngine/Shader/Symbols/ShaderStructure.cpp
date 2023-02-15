////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "depch.h"

#include <DuckEngine/Shader/Symbols/ShaderStructure.h>

#include <sstream>

namespace DuckEngine
{
	ShaderStructure::ShaderStructure(const std::string& name) :
		m_name(name),
		m_attributes()
	{

	}

	void ShaderStructure::addAttribute(const std::string type, const std::string& name)
	{
		m_attributes.push_back(std::pair<std::string, std::string>(type, name));
	}

	std::string ShaderStructure::generate() const
	{
		std::ostringstream stream;

		stream << "struct " << m_name << "\n{\n";

		for (std::pair<std::string, std::string> attribute : m_attributes)
			stream << "\t" << attribute.first << " " << attribute.second << ";\n";

		stream << "};\n";

		return stream.str();
	}
}
