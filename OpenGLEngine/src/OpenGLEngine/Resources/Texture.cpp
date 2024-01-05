#include "depch.h"
#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace OpenGLEngine
{
	Texture::Texture() :
		m_id(0),
		m_Format(0),
		m_InternalFormat(0),
		m_path(""),
		m_Width(0),
		m_Height(0)
	{
		
	}

	Texture::Texture(const std::string& path, bool gamma = false, unsigned int min_filter_param = GL_NEAREST_MIPMAP_LINEAR, unsigned int mag_filter_param = GL_NEAREST) :
		m_id(0),
		m_Format(0),
		m_InternalFormat(0),
		m_path(path)
	{
		if (path.find(".png") != std::string::npos)
		{
			stbi_set_flip_vertically_on_load(true);
		}
		else
		{
			stbi_set_flip_vertically_on_load(false);
		}

		int width, height, nbChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nbChannels, 0);

		m_Width = width;
		m_Height = height;

		if (nbChannels == 4)
		{
			m_InternalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			m_Format = GL_RGBA;
		}
		else if (nbChannels == 3)
		{
			m_InternalFormat = gamma ? GL_SRGB : GL_RGB;
			m_Format = GL_RGB;
		}

		if (!data)
			std::cout << "Failed to load texture at " << path << " : " << stbi_failure_reason() << std::endl;

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_param);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_Format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
	}

	std::string Texture::getPath() const
	{
		return m_path;
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	std::shared_ptr<Texture> Texture::CreateTexture(const std::string& path, bool gamma)
	{
		return std::make_shared<Texture>(path, gamma);
	}
}