#pragma once

#include <ShaderGenerator/Symbols/SymbolGenerator.hpp>

#include <vector>

namespace sg
{
	class SG_API ShaderStructure : public SymbolGenerator
	{
	private:
		std::string m_name;
		std::vector<std::pair<std::string, std::string>> m_attributes;

	public:
		ShaderStructure(const std::string& name);

		void addAttribute(const std::string type, const std::string& name);

		std::string generate() const override;
	};
}
