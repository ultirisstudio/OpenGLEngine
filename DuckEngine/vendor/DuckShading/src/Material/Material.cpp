#include <ShaderGenerator/Materials/Material.hpp>

namespace sg
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

	void Material::addVec3(const std::string& id, Vec3 value)
	{
		m_vec3s[id] = value;
	}

	void Material::addTexture(const std::string& id, Texture value)
	{
		m_textures[id] = value;
	}

	void Material::addCubemap(const std::string& id, Cubemap value)
	{
		m_cubemaps[id] = value;
	}

	float Material::getFloat(const std::string& id) const
	{
		return m_floats.at(id);
	}

	Vec3 Material::getVec3(const std::string& id) const
	{
		return m_vec3s.at(id);
	}

	Texture Material::getTexture(const std::string& id) const
	{
		return m_textures.at(id);
	}

	Cubemap Material::getCubemap(const std::string& id) const
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
