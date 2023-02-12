#pragma once

#include <ShaderGenerator/ShaderGeneratorExport.hpp>

namespace sg
{
	enum class SG_API ShaderVariableQualifier
	{
		NONE = -1,
		IN = 0,
		OUT,
		UNIFORM
	};
}
