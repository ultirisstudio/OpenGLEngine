#pragma once

#include <ShaderGenerator/Materials/ExternalTypes.hpp>
#include <ShaderGenerator/ShaderGeneratorExport.hpp>

#include <string>

namespace sg
{
	class SG_API MaterialProperty
	{
	private:
		float m_float;
		Vec3 m_vec3;
		Texture m_texture;
		Cubemap m_cubemap;

		bool m_isFloat;
		bool m_isVec3;
		bool m_isTexture;
		bool m_isCubemap;

	public:
		MaterialProperty();
		MaterialProperty(float value);
		MaterialProperty(Vec3 value);
		MaterialProperty(Texture value);
		MaterialProperty(Cubemap value);

		bool isFloat() const;
		bool isVec3() const;
		bool isTexture() const;
		bool isCubemap() const;

		float asFloat() const;
		Vec3 asVec3() const;
		Texture asTexture() const;
		Cubemap asCubemap() const;

		std::string getType() const;
	};
}
