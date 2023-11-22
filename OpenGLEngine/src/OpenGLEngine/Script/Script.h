#pragma once

//#include <unordered_map>

//#include <OpenGLEngine/Entity/ScriptableEntity.h>

namespace OpenGLEngine
{
	/*using script_ptr = std::unique_ptr<ScriptableEntity>;
	using script_creator = script_ptr(*)();
	using script_registery = std::unordered_map<size_t, script_creator>;
	using string_hash = std::hash<std::string>;
	
	script_registery& registery()
	{
		static script_registery reg;
		return reg;
	}

	bool register_script(size_t tag, script_creator func)
	{
		bool result{ registery().insert(script_registery::value_type(tag, func)).second };
		return result;
	}

	template <class script_class>
	script_ptr create_script()
	{
		return std::make_unique<script_class>();
	}

#define REGISTER_SCRIPT(TYPE)		\
	class TYPE;						\
	const bool reg##TYPE {			\
		register_script(			\
			string_hash()(#TYPE),	\
			create_script<TYPE>)	\
	};								\
}*/