#include "depch.h"
#include "Texture.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace OpenGLEngine
{
	Texture::Texture() :
		m_id(0)
	{
		
	}

	Texture::Texture(const std::string& path) :
		m_id(0),
		m_path(path)
	{
		unsigned int format;
		unsigned int internalFormat;

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

		if (nbChannels == 4)
		{
			format = GL_RGBA;
			internalFormat = GL_RGBA;
		}
		else if (nbChannels == 3)
		{
			format = GL_RGB;
			internalFormat = GL_RGB;
		}

		//std::cout << "Loaded texture at " << path << " with " << nbChannels << " channels" << std::endl;

		if (!data)
			std::cout << "Failed to load texture at " << path << " : " << stbi_failure_reason() << std::endl;

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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

	std::shared_ptr<Texture> Texture::CreateTexture(const std::string& path)
	{
		return std::make_shared<Texture>(path);
	}
}