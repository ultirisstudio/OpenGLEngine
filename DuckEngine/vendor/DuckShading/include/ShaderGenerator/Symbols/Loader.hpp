#pragma once

#include <ShaderGenerator/ShaderGeneratorExport.hpp>

#include <string>

namespace sg
{
	class SG_API Loader
	{
	public:
		static std::string readPiece(const std::string& path);
	};
}
