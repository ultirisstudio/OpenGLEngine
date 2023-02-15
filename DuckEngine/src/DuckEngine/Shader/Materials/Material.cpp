////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "depch.h"

#include <DuckEngine/Renderer/Renderer.h>
#include <DuckEngine/Shader/Materials/Material.h>

namespace DuckEngine
{
	Material::Material() :
		m_floats(),
		m_vec3s(),
		m_textures(),
		m_cubemaps()
	{

	}

	void Material::addFloat(const std::string& id, float value)
	{
		m_floats[id] = value;
	}

	void Material::addVec3(const std::string& id, glm::vec3 value)
	{
		m_vec3s[id] = value;
	}

	void Material::addTexture(const std::string& id, const std::string& file)
	{
		m_textures[id] = Renderer::CreateTexture(file);
	}

	void Material::addCubemap(const std::string& id, CubeMap value)
	{
		m_cubemaps[id] = value;
	}

	float Material::getFloat(const std::string& id) const
	{
		return m_floats.at(id);
	}

	glm::vec3 Material::getVec3(const std::string& id) const
	{
		return m_vec3s.at(id);
	}

	std::shared_ptr<Texture> Material::getTexture(const std::string& id) const
	{
		return m_textures.at(id);
	}

	CubeMap Material::getCubemap(const std::string& id) const
	{
		return m_cubemaps.at(id);
	}

	bool Material::hasProperty(const std::string& property) const
	{
		return hasVec3(property) || hasFloat(property) || hasTexture(property) ||hasCubemap(property);
	}

	bool Material::hasVec3(const std::string& property) const
	{
		return m_vec3s.find(property) != m_vec3s.cend();
	}
	bool Material::hasFloat(const std::string& property) const
	{
		return m_floats.find(property) != m_floats.cend();
	}
	bool Material::hasTexture(const std::string& property) const
	{
		return m_textures.find(property) != m_textures.cend();
	}

	bool Material::hasCubemap(const std::string& property) const
	{
		return m_cubemaps.find(property) != m_cubemaps.cend();
	}
}
