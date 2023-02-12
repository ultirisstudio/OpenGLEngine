#pragma once

#include <ShaderGenerator/ShaderGeneratorExport.hpp>

#include <string>

namespace sg
{
	class SG_API SymbolGenerator
	{
	public:
		virtual std::string generate() const = 0;
	};
}
