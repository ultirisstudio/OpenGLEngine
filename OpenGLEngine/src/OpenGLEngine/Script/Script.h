#pragma once

#include <unordered_map>
#include <atlsafe.h>
#include <OpenGLEngine/Entity/ScriptableEntity.h>

namespace OpenGLEngine
{
	using script_ptr = std::unique_ptr<ScriptableEntity>;
	using script_creator = script_ptr(*)();
	using script_registery = std::unordered_map<size_t, script_creator>;
	using string_hash = std::hash<std::string>;

	inline script_registery& registery()
	{
		static script_registery reg;
		return reg;
	}

	inline std::vector<std::string>& script_name()
	{
		static std::vector<std::string> names;
		return names;
	}

	extern "C" __declspec(dllexport) inline 
	std::string get_script_name(size_t index)
	{
		return script_name()[index];
	}

	extern "C" __declspec(dllexport) inline
	int get_script_name_size()
	{
		return script_name().size();
	}

	inline bool register_script(size_t tag, script_creator func)
	{
		bool result{ registery().insert(script_registery::value_type(tag, func)).second };
		return result;
	}

	template <class script_class>
	script_ptr create_script()
	{
		return std::make_unique<script_class>();
	}

	inline bool add_script_name(const char* name)
	{
		script_name().emplace_back(name);
		return true;
	}

	extern "C" __declspec(dllexport) inline
	script_creator get_script_creator(size_t tag)
	{
		auto it{ registery().find(tag) };
		if (it != registery().end())
			return it->second;
		return nullptr;
	}

	extern "C" __declspec(dllexport) inline
	LPSAFEARRAY get_script_names()
	{
		const ULONG size{ (ULONG)script_name().size() };
		if (!size) return nullptr;
		CComSafeArray<BSTR> names{ size };
		for (long i{ 0 }; i < size; ++i)
			names.SetAt(i, A2BSTR_EX(script_name()[i].c_str()), false);
		return names.Detach();
	}

#define REGISTER_SCRIPT(TYPE)																\
	const bool _reg_##TYPE { register_script(string_hash()(#TYPE), create_script<TYPE>) };	\
	const bool _name_##TYPE { add_script_name(#TYPE) };										
}