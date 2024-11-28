#include "qepch.h"
#include "Texture2D.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <fstream>

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
			case TextureFormat::RGBA: return GL_RGBA;
			case TextureFormat::SRGB: return GL_SRGB;
			case TextureFormat::SRGBA: return GL_SRGB_ALPHA;
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

	unsigned char* readFile(const std::string& filename, size_t* file_size) {
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open()) {
			return nullptr;
		}

		file.seekg(0, std::ios::end);
		std::streampos size = file.tellg();
		file.seekg(0, std::ios::beg);

		*file_size = size;

		unsigned char* data = new unsigned char[size];

		file.read((char*)data, size);
		file.close();
		return data;
	}

	Texture2D::Texture2D(const std::string& path, const TextureSpecification& specification) : Texture(specification)
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
			m_Specification.internal_format = m_Specification.gamma ? TextureFormat::SRGBA : TextureFormat::RGBA;
		}
		else
		{
			m_Specification.format = TextureFormat::RGB;
			m_Specification.internal_format = m_Specification.gamma ? TextureFormat::SRGB : TextureFormat::RGB;
		}

		int width, height, nbChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nbChannels, Utils::DesiredChannelFromTextureFormat(m_Specification.format));

		m_Specification.width = width;
		m_Specification.height = height;
		m_Specification.channels = nbChannels;

		if (!data)
			std::cout << "Failed to load texture from memory " << " : " << stbi_failure_reason() << "\n" << path << std::endl;

		bool multisample = m_Specification.Samples > 1;

		glCreateTextures(Utils::TextureTarget(multisample), 1, &m_ID);
		glBindTexture(Utils::TextureTarget(multisample), m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, Utils::TextureWrapToGL(m_Specification.wrap_r));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Utils::TextureWrapToGL(m_Specification.wrap_s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Utils::TextureWrapToGL(m_Specification.wrap_t));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Utils::TextureFilterToGL(m_Specification.min_filter_param));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Utils::TextureFilterToGL(m_Specification.mag_filter_param));

		glTexImage2D(GL_TEXTURE_2D, 0, Utils::TextureFormatToGL(m_Specification.internal_format), m_Specification.width, m_Specification.height, 0, Utils::TextureFormatToGL(m_Specification.format), GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	Texture2D::Texture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification) : Texture(specification)
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
			m_Specification.internal_format = m_Specification.gamma ? TextureFormat::SRGBA : TextureFormat::RGBA;
		}
		else
		{
			m_Specification.format = TextureFormat::RGB;
			m_Specification.internal_format = m_Specification.gamma ? TextureFormat::SRGB : TextureFormat::RGB;
		}

		int width, height, nbChannels;
		unsigned char* data = stbi_load_from_memory(image_data, size, &width, &height, &nbChannels, Utils::DesiredChannelFromTextureFormat(m_Specification.format));

		m_Specification.width = width;
		m_Specification.height = height;
		m_Specification.channels = nbChannels;

		if (!data)
		{
			std::cout << "Failed to load texture from memory " << " : " << stbi_failure_reason() << std::endl;
		}
		else
		{
			bool multisample = m_Specification.Samples > 1;

			glCreateTextures(Utils::TextureTarget(multisample), 1, &m_ID);
			glBindTexture(Utils::TextureTarget(multisample), m_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, Utils::TextureWrapToGL(m_Specification.wrap_r));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Utils::TextureWrapToGL(m_Specification.wrap_s));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Utils::TextureWrapToGL(m_Specification.wrap_t));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Utils::TextureFilterToGL(m_Specification.min_filter_param));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Utils::TextureFilterToGL(m_Specification.mag_filter_param));

			glTexImage2D(GL_TEXTURE_2D, 0, Utils::TextureFormatToGL(m_Specification.internal_format), m_Specification.width, m_Specification.height, 0, Utils::TextureFormatToGL(m_Specification.format), GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(image_data);
		}
	}

	unsigned char* Texture2D::LoadDataFromPath(const std::string& path, size_t* file_size)
	{
		return readFile(path, file_size);
	}

	void Texture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	std::shared_ptr<Texture2D> Texture2D::CreateTexture2D(const std::string& path, const TextureSpecification& specification)
	{
		return std::make_shared<Texture2D>(path, specification);
	}

	std::shared_ptr<Texture2D> Texture2D::CreateTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification)
	{
		return std::make_shared<Texture2D>(image_data, size, specification);
	}
}