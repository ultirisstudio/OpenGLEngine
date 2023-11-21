#pragma once

#include <unordered_map>

#include <OpenGLEngine/Entity/ScriptableEntity.h>

namespace OpenGLEngine
{
	using script_registery = std::unordered_map<size_t, ScriptableEntity>;

	
	script_registery& registery()
	{
		static script_registery reg;
		return reg;
	}

	unsigned int registerScript(size_t tag, ScriptableEntity func)
	{
		bool result{ registery().insert(script_registery::value_type(tag, func)).second };
		return result;
	}
}