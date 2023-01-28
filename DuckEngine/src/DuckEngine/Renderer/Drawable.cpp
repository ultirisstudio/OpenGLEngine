#include "depch.h"
#include <DuckEngine/Renderer/Drawable.h>

namespace DuckEngine
{
	Drawable::Drawable(Material& material, Shader& shader) : m_Material(&material), m_Shader(&shader)
	{
		
	}

	Material& Drawable::getMaterial() const
	{
		return *m_Material;
	}
	Shader& Drawable::getShader() const
	{
		return *m_Shader;
	}
}