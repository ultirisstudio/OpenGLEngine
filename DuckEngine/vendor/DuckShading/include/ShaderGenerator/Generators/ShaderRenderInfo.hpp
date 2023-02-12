#pragma once

#include <ShaderGenerator/ShaderGeneratorExport.hpp>

#include <string>
#include <vector>

namespace sg
{
	class SG_API ShaderRenderInfo
	{
	private:
		std::vector<std::pair<std::string, std::string>> m_renderInfo;

	public:
		ShaderRenderInfo();

		void addRenderInfo(const std::string& name, const std::string& value);

		std::vector<std::pair<std::string, std::string>> getRenderInfo() const;
	};
}
