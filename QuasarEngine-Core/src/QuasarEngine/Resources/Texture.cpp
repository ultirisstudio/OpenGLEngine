#include "qepch.h"
#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <fstream>

namespace QuasarEngine
{
	Texture::Texture(const TextureSpecification& specification) : m_ID(0), m_Specification(specification)
	{
		
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}
}