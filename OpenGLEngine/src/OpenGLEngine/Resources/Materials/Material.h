////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <OpenGLEngine/Resources/Materials/CubeMap.h>
#include <OpenGLEngine/Resources/Texture.h>

#include <glm/glm.hpp>

#include <map>
#include <string>

namespace OpenGLEngine
{
	class Material
	{
	private:
		std::map<std::string, std::shared_ptr<float>> m_floats;
		std::map<std::string, std::shared_ptr<glm::vec3>> m_vec3s;
		std::map<std::string, std::shared_ptr<Texture>> m_textures;
		std::map<std::string, std::shared_ptr<CubeMap>> m_cubemaps;
		std::map<std::string, std::shared_ptr<bool>> m_booleans;

	public:
		Material();

		void addFloat(const std::string& id, float value);
		void addVec3(const std::string& id, glm::vec3 value);
		void addTexture(const std::string& id, const std::string& file);
		void addCubemap(const std::string& id, CubeMap value);
		void addBoolean(const std::string& id, bool value);

		std::shared_ptr<float> getFloat(const std::string& id) const;
		std::shared_ptr<glm::vec3> getVec3(const std::string& id) const;
		std::shared_ptr<Texture> getTexture(const std::string& id) const;
		std::shared_ptr<CubeMap> getCubemap(const std::string& id) const;
		std::shared_ptr<bool> getBoolean(const std::string& id) const;

		bool hasProperty(const std::string& id) const;
		bool hasVec3(const std::string& id) const;
		bool hasFloat(const std::string& id) const;
		bool hasTexture(const std::string& id) const;
		bool hasCubemap(const std::string& id) const;
		bool hasBoolean(const std::string& id) const;

		static std::shared_ptr<Material> CreateMaterial();
	};
}
