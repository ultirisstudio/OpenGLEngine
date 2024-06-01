#pragma once

#include <OpenGLEngine/Resources/Texture.h>
#include <OpenGLEngine/Resources/Model.h>
#include <OpenGLEngine/Resources/Materials/Material.h>

#include <map>
#include <string>

namespace OpenGLEngine
{
	class ResourceManager
	{
	private:
		std::map<std::string, std::shared_ptr<Texture>> m_Textures;
		std::map<std::string, std::shared_ptr<Model>> m_Models;

	public:
		std::shared_ptr<Texture> GetTexture(const std::string& id);
		std::shared_ptr<Model> GetModel(const std::string& id);

		std::shared_ptr<Texture> CreateTexture(const std::string& id, const TextureSpecification& specification);
		std::shared_ptr<Model> CreateModel(const std::string& id);

		std::shared_ptr<Texture> UpdateTexture(const std::string& id, const TextureSpecification& specification);

		int GetLoadedTexture() const;
		int GetLoadedModel() const;

		void Reset();
	};
}