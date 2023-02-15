////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DuckEngine/Shader/Symbols/ShaderVariableQualifier.hpp>
#include <DuckEngine/Shader/Symbols/SymbolGenerator.h>

#include <vector>

namespace DuckEngine
{
	class ShaderVariable : public SymbolGenerator
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
