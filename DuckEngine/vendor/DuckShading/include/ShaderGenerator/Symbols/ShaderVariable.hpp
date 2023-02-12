#pragma once

#include <ShaderGenerator/Symbols/ShaderVariableQualifier.hpp>
#include <ShaderGenerator/Symbols/SymbolGenerator.hpp>

#include <vector>

namespace sg
{
	class SG_API ShaderVariable : public SymbolGenerator
	{
	private:
		std::string m_name;
		std::string m_type;
		ShaderVariableQualifier m_qualifier;
		int m_location;

	public:
		ShaderVariable(const std::string& name, const std::string type, ShaderVariableQualifier qualifier, int location = -1);

		std::string generate() const override;
	};
}
