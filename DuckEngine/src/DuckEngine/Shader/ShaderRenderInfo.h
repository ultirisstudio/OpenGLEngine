#pragma once

#include <map>
#include <string>

namespace DuckEngine
{
	struct ShaderRenderInfo
	{
		std::map<std::string, std::string> uniforms;
	};
}