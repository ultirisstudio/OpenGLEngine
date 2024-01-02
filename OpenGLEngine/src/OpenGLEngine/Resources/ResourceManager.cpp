#include "depch.h"

#include <glad/glad.h>
#include <OpenGLEngine/Resources/ResourceManager.h>

namespace OpenGLEngine
{
	std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& id)
	{
		if (m_Textures.find(id) != m_Textures.cend())
		{
			return m_Textures.at(id);
		}
		else
		{
			m_Textures[id] = Texture::CreateTexture(id);
			return m_Textures.at(id);
		}
	}

	std::shared_ptr<Material> ResourceManager::getMaterial(const std::string& id)
	{
		if (m_Materials.find(id) != m_Materials.cend())
		{
			return m_Materials.at(id);
		}
		else
		{
			m_Materials[id] = Material::CreateMaterial();
			return m_Materials.at(id);
		}
	}

	std::shared_ptr<Model> ResourceManager::getModel(const std::string& id)
	{
		if (m_Models.find(id) != m_Models.cend())
		{
			return m_Models.at(id);
		}
		else
		{
			m_Models[id] = Model::CreateModel(id);
			return m_Models.at(id);
		}
	}
}