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
		std::map<std::string, std::shared_ptr<Material>> m_Materials;
		std::map<std::string, std::shared_ptr<Texture>> m_Textures;
		std::map<std::string, std::shared_ptr<Model>> m_Models;

	public:
		std::shared_ptr<Texture> getTexture(const std::string& id);
		std::shared_ptr<Material> getMaterial(const std::string& id);
		std::shared_ptr<Model> getModel(const std::string& id);
	};
}