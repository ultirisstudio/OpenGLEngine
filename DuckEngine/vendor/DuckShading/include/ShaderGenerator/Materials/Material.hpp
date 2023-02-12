#pragma once

#include <ShaderGenerator/Materials/ExternalTypes.hpp>
#include <ShaderGenerator/ShaderGeneratorExport.hpp>

#include <map>
#include <string>

namespace sg
{
	class SG_API Material
	{
	private:
		std::map<std::string, float> m_floats;
		std::map<std::string, Vec3> m_vec3s;
		std::map<std::string, Texture> m_textures;
		std::map<std::string, Cubemap> m_cubemaps;

	public:
		Material();

		void addFloat(const std::string& id, float value);
		void addVec3(const std::string& id, Vec3 value);
		void addTexture(const std::string& id, Texture value);
		void addCubemap(const std::string& id, Cubemap value);

		float getFloat(const std::string& id) const;
		Vec3 getVec3(const std::string& id) const;
		Texture getTexture(const std::string& id) const;
		Cubemap getCubemap(const std::string& id) const;

		bool hasProperty(const std::string& id) const;
		bool hasVec3(const std::string& id) const;
		bool hasFloat(const std::string& id) const;
		bool hasTexture(const std::string& id) const;
		bool hasCubemap(const std::string& id) const;
	};
}
