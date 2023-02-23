#include "depch.h"
#include <DuckEngine/Resources/ResourceManager.h>

namespace DuckEngine
{
	std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& id)
	{
		if (m_Textures.find(id) != m_Textures.cend())
		{
			//std::cout << "Texture charged with: " << id << std::endl;
			return m_Textures.at(id);
		}
		else
		{
			//std::cout << "Texture created with: " << id << std::endl;
			m_Textures[id] = Texture::CreateTexture(id);
			return m_Textures.at(id);
		}
	}

	std::shared_ptr<Material> ResourceManager::getMaterial(const std::string& id)
	{
		if (m_Materials.find(id) != m_Materials.cend())
		{
			//std::cout << "Material charged with: " << id << std::endl;
			return m_Materials.at(id);
		}
		else
		{
			//std::cout << "Material created with: " << id << std::endl;
			m_Materials[id] = Material::CreateMaterial();
			return m_Materials.at(id);
		}
	}

	std::shared_ptr<Model> ResourceManager::getModel(const std::string& id)
	{
		if (m_Models.find(id) != m_Models.cend())
		{
			//std::cout << "Model charged with: " << id << std::endl;
			return m_Models.at(id);
		}
		else
		{
			//std::cout << "Model created with: " << id << std::endl;
			m_Models[id] = Model::CreateModel(id);
			return m_Models.at(id);
		}
	}
}