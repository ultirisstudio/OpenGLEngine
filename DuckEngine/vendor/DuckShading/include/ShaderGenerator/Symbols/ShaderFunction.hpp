#pragma once

#include <ShaderGenerator/Symbols/SymbolGenerator.hpp>

#include <vector>

namespace sg
{
	class SG_API ShaderFunction : public SymbolGenerator
	{
	private:
		std::string m_name;
		std::string m_type;
		std::vector<std::string> m_instructions;

	public:
		ShaderFunction(const std::string& name, const std::string type);

		void addInstruction(const std::string instruction);

		std::string generate() const override;
	};
}
