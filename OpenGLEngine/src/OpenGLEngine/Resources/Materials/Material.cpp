////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "depch.h"

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Resources/Materials/Material.h>

namespace OpenGLEngine
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
		m_floats[id] = std::make_shared<float>(value);
	}

	void Material::addVec3(const std::string& id, glm::vec3 value)
	{
		m_vec3s[id] = std::make_shared<glm::vec3>(value);
	}

	void Material::addTexture(const std::string& id, const std::string& file)
	{
		//m_textures[id] = Texture::CreateTexture(file);
		m_textures[id] = Renderer::m_SceneData.m_ResourceManager.getTexture(file);
	}

	void Material::addCubemap(const std::string& id, CubeMap value)
	{
		m_cubemaps[id] = std::make_shared<CubeMap>();
	}

	void Material::addBoolean(const std::string& id, bool value)
	{
		m_booleans[id] = std::make_shared<bool>(value);
	}

	std::shared_ptr<float> Material::getFloat(const std::string& id) const
	{
		return m_floats.at(id);
	}

	std::shared_ptr<glm::vec3> Material::getVec3(const std::string& id) const
	{
		return m_vec3s.at(id);
	}

	std::shared_ptr<Texture> Material::getTexture(const std::string& id) const
	{
		return m_textures.at(id);
	}

	std::shared_ptr<CubeMap> Material::getCubemap(const std::string& id) const
	{
		return m_cubemaps.at(id);
	}

	std::shared_ptr<bool> Material::getBoolean(const std::string& id) const
	{
		return m_booleans.at(id);
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

	bool Material::hasBoolean(const std::string& property) const
	{
		return m_booleans.find(property) != m_booleans.cend();
	}

	std::shared_ptr<Material> Material::CreateMaterial()
	{
		return std::make_shared<Material>();
	}
}
