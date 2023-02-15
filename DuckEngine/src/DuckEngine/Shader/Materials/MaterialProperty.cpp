////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////    --CREDITS: BROCOLARBRE--    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "depch.h"

#include <DuckEngine/Shader/Materials/MaterialProperty.h>

namespace DuckEngine
{
	MaterialProperty::MaterialProperty() :
		m_float(0.0f),
		m_vec3(),
		m_texture(),
		m_cubemap(),
		m_isFloat(true),
		m_isVec3(false),
		m_isTexture(false),
		m_isCubemap(false)
	{

	}

	MaterialProperty::MaterialProperty(float value) :
		m_float(value),
		m_vec3(),
		m_texture(),
		m_cubemap(),
		m_isFloat(true),
		m_isVec3(false),
		m_isTexture(false),
		m_isCubemap(false)
	{

	}

	MaterialProperty::MaterialProperty(glm::vec3 value) :
		m_float(),
		m_vec3(value),
		m_texture(),
		m_cubemap(),
		m_isFloat(false),
		m_isVec3(true),
		m_isTexture(false),
		m_isCubemap(false)
	{

	};

	MaterialProperty::MaterialProperty(Texture* value) :
		m_float(),
		m_vec3(),
		m_texture(value),
		m_cubemap(),
		m_isFloat(false),
		m_isVec3(false),
		m_isTexture(true),
		m_isCubemap(false)
	{

	};

	MaterialProperty::MaterialProperty(CubeMap value) :
		m_float(),
		m_vec3(),
		m_texture(),
		m_cubemap(value),
		m_isFloat(false),
		m_isVec3(false),
		m_isTexture(false),
		m_isCubemap(true)
	{

	};

	bool MaterialProperty::isFloat() const
	{
		return m_isFloat;
	}

	bool MaterialProperty::isVec3() const
	{
		return m_isVec3;
	}

	bool MaterialProperty::isTexture() const
	{
		return m_isTexture;
	}

	bool MaterialProperty::isCubemap() const
	{
		return m_isCubemap;
	}

	float MaterialProperty::asFloat() const
	{
		return m_float;
	}

	glm::vec3 MaterialProperty::asVec3() const
	{
		return m_vec3;
	}

	Texture* MaterialProperty::asTexture() const
	{
		return m_texture;
	}

	CubeMap MaterialProperty::asCubemap() const
	{
		return m_cubemap;
	}

	std::string MaterialProperty::getType() const
	{
		if (m_isFloat)
			return "float";

		if (m_isTexture)
			return "sampler2D";

		if (m_isVec3)
			return "vec3";

		if (m_isCubemap)
			return "samplerCube";

		return "";
	}
}
