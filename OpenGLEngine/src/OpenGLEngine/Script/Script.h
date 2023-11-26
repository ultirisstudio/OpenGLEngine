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

	script_registery& registery()
	{
		static script_registery reg;
		return reg;
	}

	std::vector<std::string>& script_name()
	{
		static std::vector<std::string> names;
		return names;
	}

	extern "C" __declspec(dllexport)
	std::string get_script_name(size_t index)
	{
		return script_name()[index];
	}

	extern "C" __declspec(dllexport)
	int get_script_name_size()
	{
		return script_name().size();
	}

	bool register_script(size_t tag, script_creator func)
	{
		std::cout << "Registered script: " << tag << " - Map size: " << registery().size() << std::endl;
		bool result{ registery().insert(script_registery::value_type(tag, func)).second };
		std::cout << "Result: " << result << " - Map size: " << registery().size() << std::endl;
		return result;
	}

	template <class script_class>
	script_ptr create_script()
	{
		std::cout << "Created script: " << typeid(script_class).name() << std::endl;
		return std::make_unique<script_class>();
	}

	bool add_script_name(const char* name)
	{
		std::cout << "Add script name: " << name << std::endl;
		script_name().emplace_back(name);
		return true;
	}

	extern "C" __declspec(dllexport)
	script_creator get_script_creator(size_t tag)
	{
		auto it{ registery().find(tag) };
		if (it != registery().end())
			return it->second;
		return nullptr;
	}

	extern "C" __declspec(dllexport)
	LPSAFEARRAY get_script_names()
	{
		const ULONG size{ (ULONG)script_name().size() };
		if (!size) return nullptr;
		CComSafeArray<BSTR> names{ size };
		for (long i{ 0 }; i < size; ++i)
			names.SetAt(i, A2BSTR_EX(script_name()[i].c_str()), false);
		return names.Detach();
	}

	extern "C" __declspec(dllexport)
	bool test()
	{
		std::cout << "Test" << std::endl;
		return true;
	}

#define REGISTER_SCRIPT(TYPE)																\
	const bool _test_##TYPE { test() };														\
	const bool _reg_##TYPE { register_script(string_hash()(#TYPE), create_script<TYPE>) };	\
	const bool _name_##TYPE { add_script_name(#TYPE) };										\
	const bool _init_##TYPE { TYPE::Init() };
}