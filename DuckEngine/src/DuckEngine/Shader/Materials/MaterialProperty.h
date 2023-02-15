////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <glm/glm.hpp>
#include <DuckEngine/Resources/Texture.h>
#include <DuckEngine/Resources/CubeMap.h>

namespace DuckEngine
{
	class MaterialProperty
	{
	private:
		float m_float;
		glm::vec3 m_vec3;
		Texture* m_texture;
		CubeMap m_cubemap;

		bool m_isFloat;
		bool m_isVec3;
		bool m_isTexture;
		bool m_isCubemap;

	public:
		MaterialProperty();
		MaterialProperty(float value);
		MaterialProperty(glm::vec3 value);
		MaterialProperty(Texture* value);
		MaterialProperty(CubeMap value);

		bool isFloat() const;
		bool isVec3() const;
		bool isTexture() const;
		bool isCubemap() const;

		float asFloat() const;
		glm::vec3 asVec3() const;
		Texture* asTexture() const;
		CubeMap asCubemap() const;

		std::string getType() const;
	};
}
