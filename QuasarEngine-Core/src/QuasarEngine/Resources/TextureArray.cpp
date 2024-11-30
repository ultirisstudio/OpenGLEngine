#include "qepch.h"
#include "TextureArray.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace QuasarEngine
{
	namespace Utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static GLenum TextureFormatToGL(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB: return GL_RGB;
			case TextureFormat::RGB8: return GL_RGB8;
			case TextureFormat::RGBA: return GL_RGBA;
			case TextureFormat::RGBA8: return GL_RGBA8;
			case TextureFormat::SRGB: return GL_SRGB;
			case TextureFormat::SRGB8: return GL_SRGB8;
			case TextureFormat::SRGBA: return GL_SRGB_ALPHA;
			case TextureFormat::SRGB8A8: return GL_SRGB8_ALPHA8;
			case TextureFormat::RED: return GL_RED;
			}
			return 0;
		}


		static GLenum TextureWrapToGL(TextureWrap wrap)
		{
			switch (wrap)
			{
			case TextureWrap::REPEAT: return GL_REPEAT;
			case TextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
			case TextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
			case TextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
			}
			return 0;
		}

		static GLenum TextureFilterToGL(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::NEAREST: return GL_NEAREST;
			case TextureFilter::LINEAR: return GL_LINEAR;
			case TextureFilter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilter::LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilter::NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilter::LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
			}
			return 0;
		}

		static int DesiredChannelFromTextureFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB: return 3;
			case TextureFormat::RGBA: return 4;
			case TextureFormat::RED: return 1;
			}
			return 0;
		}
	}

	TextureArray::TextureArray(const std::vector<std::string>& paths, const TextureSpecification& specification) : Texture(specification)
	{
		if (m_Specification.flip)
		{
			stbi_set_flip_vertically_on_load(true);
		}
		else
		{
			stbi_set_flip_vertically_on_load(false);
		}

		if (m_Specification.alpha)
		{
			m_Specification.format = TextureFormat::RGBA;
			m_Specification.internal_format = m_Specification.gamma ? TextureFormat::SRGB8A8 : TextureFormat::RGBA8;
		}
		else
		{
			m_Specification.format = TextureFormat::RGB;
			m_Specification.internal_format = m_Specification.gamma ? TextureFormat::SRGB8 : TextureFormat::RGB8;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
		
		//GL_SRGB8
		//GL_SRGB8_ALPHA8
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, Utils::TextureFormatToGL(m_Specification.internal_format), 32, 32, static_cast<GLsizei>(paths.size()));
		for (unsigned int i = 0; i < paths.size(); ++i)
		{
			int width, height, nbChannels;
			unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nbChannels, Utils::DesiredChannelFromTextureFormat(m_Specification.format));

			if (!data)
				std::cout << "Failed to load texture at " << paths[i] << " : " << stbi_failure_reason() << std::endl;

			//GL_RGB
			//GL_RGBA
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, Utils::TextureFormatToGL(m_Specification.format), GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		/*GLfloat largest_supported_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &largest_supported_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, largest_supported_anisotropy);*/

		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

	std::shared_ptr<TextureArray> TextureArray::CreateTextureArray(const std::vector<std::string>& paths, const TextureSpecification& specification)
	{
		return std::make_shared<TextureArray>(paths, specification);
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