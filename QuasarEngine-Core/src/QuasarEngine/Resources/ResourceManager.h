#pragma once

#include <map>
#include <string>
#include <queue>
#include <mutex>
#include <thread>

#include <QuasarEngine/Resources/Texture.h>
#include <QuasarEngine/Resources/Model.h>

namespace QuasarEngine
{
	class ResourceManager
	{
	private:
		std::map<std::string, std::shared_ptr<Texture>> m_Textures;
		std::map<std::string, std::shared_ptr<Model>> m_Models;

		std::vector<std::string> m_WaitingTextures;

	private:
		struct TextureInfos
		{
			std::string path;
			TextureSpecification specifications;
			std::vector<unsigned char> data;
		};

		std::queue<TextureInfos> m_LoadingTexturesQueue;
		std::queue<TextureInfos> m_LoadingDataQueue;

		std::mutex m_LoadingTexture;
		std::mutex m_LoadingData;

		std::condition_variable m_Condition;
		std::condition_variable m_ConditionTexture;

	public:
		ResourceManager();
		~ResourceManager();

		void Update();
		void ResourceLoader();
		void Reset();

		std::shared_ptr<Texture> GetTexture(const std::string& id);
		std::shared_ptr<Model> GetModel(const std::string& id);

		std::shared_ptr<Texture> CreateTexture(const std::string& id, const TextureSpecification& specification);
		std::shared_ptr<Model> CreateModel(const std::string& id);

		void mt_CreateTexture(const std::string& id, const TextureSpecification& specification);

		std::shared_ptr<Texture> UpdateTexture(const std::string& id, const TextureSpecification& specification);
	};
}