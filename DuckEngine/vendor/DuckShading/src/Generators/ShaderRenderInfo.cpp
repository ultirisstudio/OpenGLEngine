#include <ShaderGenerator/Generators/ShaderRenderInfo.hpp>

namespace sg
{
	ShaderRenderInfo::ShaderRenderInfo() :
		m_renderInfo()
	{

	}

	void ShaderRenderInfo::addRenderInfo(const std::string& name, const std::string& value)
	{
		m_renderInfo.push_back(std::pair<std::string, std::string>(name, value));
	}

	std::vector<std::pair<std::string, std::string>> ShaderRenderInfo::getRenderInfo() const
	{
		return m_renderInfo;
	}
}