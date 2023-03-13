////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <OpenGLEngine/Shader/Symbols/ShaderVariableQualifier.hpp>
#include <OpenGLEngine/Shader/Symbols/SymbolGenerator.h>

#include <vector>

namespace OpenGLEngine
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
