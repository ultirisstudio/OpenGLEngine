#include "qepch.h"

#include <glad/glad.h>
#include <QuasarEngine/Resources/ResourceManager.h>
#include <QuasarEngine/Resources/Materials/Material.h>

namespace QuasarEngine
{
	ResourceManager::ResourceManager()
	{
		std::thread t(&ResourceManager::ResourceLoader, this);
		t.detach();
	}

	ResourceManager::~ResourceManager()
	{

	}

	void ResourceManager::Update()
	{
		std::unique_lock<std::mutex> lock_texture(m_LoadingTexture, std::try_to_lock);
		if (lock_texture.owns_lock()) {
			if (!m_LoadingTexturesQueue.empty())
			{
				TextureInfos infos = m_LoadingTexturesQueue.front();
				m_LoadingTexturesQueue.pop();

				lock_texture.unlock();

				m_Textures[infos.path] = std::make_shared<Texture>(infos.data, infos.size, infos.specifications);
			}
		}
	}

	void ResourceManager::ResourceLoader()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_LoadingData);

			//std::cout << "Ressource Manager Thread wait texture" << std::endl;

			m_Condition.wait(lock, [this]() {
				return !m_LoadingDataQueue.empty();
			});

			/*if (m_LoadingDataQueue.empty()) {
				lock.unlock();
				continue;
			}*/

			TextureInfos infos = m_LoadingDataQueue.front();
			//m_LoadingDataQueue.pop();
			lock.unlock();

			//std::cout << infos.path << " loading..." << std::endl;

			infos.data = Texture::LoadDataFromPath(infos.path, &infos.size);

			//std::unique_lock<std::mutex> lock_texture(m_LoadingTexture);

			//m_ConditionTexture.wait(lock_texture);
			//m_LoadingTexturesQueue.push(infos);

			//lock_texture.unlock();

			std::unique_lock<std::mutex> lock_texture(m_LoadingTexture, std::try_to_lock);
			if (lock_texture.owns_lock()) {
				m_LoadingTexturesQueue.push(infos);
				m_LoadingDataQueue.pop();
				lock_texture.unlock();
			}

			//lock_texture.unlock();
		}
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

	void ResourceManager::mt_CreateTexture(const std::string& id, const TextureSpecification& specification)
	{
		//std::cout << "Editor want load: " << id << std::endl;
		std::vector<std::string>::iterator it = std::find(m_WaitingTextures.begin(), m_WaitingTextures.end(), id);
		if ((m_Textures.find(id) == m_Textures.cend()) && it == m_WaitingTextures.end())
		{
			m_WaitingTextures.push_back(id);

			std::unique_lock<std::mutex> lock(m_LoadingData);

			TextureInfos infos;
			infos.path = id;
			infos.specifications = specification;
			m_LoadingDataQueue.push(infos);
			lock.unlock();

			//std::cout << "Texture added to loading queue" << std::endl;
			
			m_Condition.notify_one();
		}
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
}