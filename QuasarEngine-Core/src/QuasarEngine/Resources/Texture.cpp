#include "qepch.h"
#include "Texture.h"

#include <fstream>

#include <QuasarEngine/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLTexture2D.h>

namespace QuasarEngine
{
	Texture::Texture(const TextureSpecification& specification) : m_ID(0), m_Specification(specification)
	{
		
	}
}