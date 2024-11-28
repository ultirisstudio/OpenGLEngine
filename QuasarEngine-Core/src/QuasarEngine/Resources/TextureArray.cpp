#include "qepch.h"
#include "TextureArray.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace QuasarEngine
{
	TextureArray::TextureArray(const std::vector<std::string>& paths, const TextureSpecification& specification) : Texture(specification)
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 32, 32, static_cast<GLsizei>(paths.size()));
		for (unsigned int i = 0; i < paths.size(); ++i)
		{
			int width, height, nbChannels;
			unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nbChannels, 0);

			if (!data)
				std::cout << "Failed to load texture at " << paths[i] << " : " << stbi_failure_reason() << std::endl;

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		GLfloat largest_supported_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &largest_supported_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, largest_supported_anisotropy);

		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

	TextureArray::TextureArray(const std::vector<unsigned char*>& files, const std::vector<size_t>& sizes, const TextureSpecification& specification) : Texture(specification)
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 32, 32, static_cast<GLsizei>(files.size()));
		for (unsigned int i = 0; i < files.size(); ++i)
		{
			int width, height, nbChannels;
			unsigned char* data = stbi_load_from_memory(files[i], sizes[i], &width, &height, &nbChannels, 0);

			if (!data)
				std::cout << "Failed to load texture at " << files[i] << " : " << stbi_failure_reason() << std::endl;

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		GLfloat largest_supported_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &largest_supported_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, largest_supported_anisotropy);

		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

	std::shared_ptr<TextureArray> TextureArray::CreateTextureArray(const std::vector<std::string>& paths, const TextureSpecification& specification)
	{
		return std::make_shared<TextureArray>(paths, specification);
	}

	std::shared_ptr<TextureArray> TextureArray::CreateTextureArray(const std::vector<unsigned char*>& files, const std::vector<size_t>& sizes, const TextureSpecification& specification)
	{
		return std::make_shared<TextureArray>(files, sizes, specification);
	}

	void TextureArray::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
	}

	void TextureArray::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
}