#include "qepch.h"

#include <glad/glad.h>
#include <QuasarEngine/Resources/ResourceManager.h>
#include <QuasarEngine/Resources/Materials/Material.h>

namespace QuasarEngine
{
	std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& id)
	{
		if (m_Textures.find(id) != m_Textures.cend())
			return m_Textures.at(id);
		else
			return nullptr;
	}

	std::shared_ptr<Model> ResourceManager::GetModel(const std::string& id)
	{
		if (m_Models.find(id) != m_Models.cend())
			return m_Models.at(id);
		else
			return nullptr;
	}

	std::shared_ptr<Texture> ResourceManager::CreateTexture(const std::string& id, const TextureSpecification& specification)
	{
		if (m_Textures.find(id) != m_Textures.cend())
			return m_Textures.at(id);
		
		m_Textures[id] = std::make_shared<Texture>(id, specification);
		return m_Textures[id];
	}

	std::shared_ptr<Model> ResourceManager::CreateModel(const std::string& id)
	{
		if (m_Models.find(id) != m_Models.cend())
			return m_Models.at(id);

		m_Models[id] = std::make_shared<Model>(id);
		return m_Models[id];
	}

	std::shared_ptr<Texture> ResourceManager::UpdateTexture(const std::string& id, const TextureSpecification& specification)
	{
		if (m_Textures.find(id) != m_Textures.cend())
		{
			m_Textures[id] = std::make_shared<Texture>(id, specification);
			return m_Textures[id];
		}

		return nullptr;
	}

	void ResourceManager::Reset()
	{
		for (auto& texture : m_Textures)
		{
			texture.second.reset();
		}

		for (auto& model : m_Models)
		{
			model.second.reset();
		}

		m_Textures.clear();
		m_Models.clear();
	}
}