#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <OpenGLEngine/Asset/Asset.h>

namespace OpenGLEngine
{
	enum class TextureFormat : uint8_t
	{
		RED = 0,
		RGB,
		RGBA,
		SRGB,
		SRGBA,
	};

	enum class TextureWrap : uint8_t
	{
		REPEAT = 0,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class TextureFilter : uint8_t
	{
		NEAREST = 0,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR
	};

	struct TextureSpecification
	{
		TextureFormat format = TextureFormat::RGB;
		TextureFormat internal_format = TextureFormat::RGB;

		TextureWrap wrap_r = TextureWrap::REPEAT;
		TextureWrap wrap_s = TextureWrap::REPEAT;
		TextureWrap wrap_t = TextureWrap::REPEAT;

		TextureFilter min_filter_param = TextureFilter::NEAREST_MIPMAP_LINEAR;
		TextureFilter mag_filter_param = TextureFilter::NEAREST;

		uint32_t width = 0, height = 0;

		bool alpha = false;
		bool gamma = false;
		bool flip = false;
		bool mipmap = true; // TODO : implement this
		bool compressed = false; // TODO : implement this

		uint32_t Samples = 1;
		uint32_t channels = 3;
	};

	class Texture : public Asset
	{
	private:
		uint32_t m_ID;
		TextureSpecification m_Specification;
	public:
		Texture(const std::string& path, const TextureSpecification& specification);
		Texture(unsigned char* image_data, const TextureSpecification& specification);
		~Texture();

		static std::shared_ptr<Texture> CreateTexture(const std::string& path, const TextureSpecification& specification);
		static std::shared_ptr<Texture> CreateTexture(unsigned char* image_data, const TextureSpecification& specification);

		unsigned int GetID() const { return m_ID; }

		const TextureSpecification& GetSpecification() const { return m_Specification; }

		void Bind() const;
		void Unbind() const;

		static AssetType GetStaticType() { return AssetType::TEXTURE; }
		virtual AssetType GetType() override { return GetStaticType(); }
	};

	namespace Utils
	{
		static const char* TextureWrapToChar(TextureWrap wrap)
		{
			switch (wrap)
			{
			case TextureWrap::REPEAT: return "Repeat";
			case TextureWrap::MIRRORED_REPEAT: return "Mirrored Repeat";
			case TextureWrap::CLAMP_TO_EDGE: return "Clamp to Edge";
			case TextureWrap::CLAMP_TO_BORDER: return "Clamp to Border";
			}
			return "Unknown";
		}

		static const char* TextureFilterToChar(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::NEAREST: return "Nearest";
			case TextureFilter::LINEAR: return "Linear";
			case TextureFilter::NEAREST_MIPMAP_NEAREST: return "Nearest Mipmap Nearest";
			case TextureFilter::LINEAR_MIPMAP_NEAREST: return "Linear Mipmap Nearest";
			case TextureFilter::NEAREST_MIPMAP_LINEAR: return "Nearest Mipmap Linear";
			case TextureFilter::LINEAR_MIPMAP_LINEAR: return "Linear Mipmap Linear";
			}
			return "Unknown";
		}

		static TextureWrap CharToTextureWrap(const char* wrap)
		{
			if (strcmp(wrap, "Repeat") == 0) return TextureWrap::REPEAT;
			if (strcmp(wrap, "Mirrored Repeat") == 0) return TextureWrap::MIRRORED_REPEAT;
			if (strcmp(wrap, "Clamp to Edge") == 0) return TextureWrap::CLAMP_TO_EDGE;
			if (strcmp(wrap, "Clamp to Border") == 0) return TextureWrap::CLAMP_TO_BORDER;
			return TextureWrap::REPEAT;
		}

		static TextureFilter CharToTextureFilter(const char* filter)
		{
			if (strcmp(filter, "Nearest") == 0) return TextureFilter::NEAREST;
			if (strcmp(filter, "Linear") == 0) return TextureFilter::LINEAR;
			if (strcmp(filter, "Nearest Mipmap Nearest") == 0) return TextureFilter::NEAREST_MIPMAP_NEAREST;
			if (strcmp(filter, "Linear Mipmap Nearest") == 0) return TextureFilter::LINEAR_MIPMAP_NEAREST;
			if (strcmp(filter, "Nearest Mipmap Linear") == 0) return TextureFilter::NEAREST_MIPMAP_LINEAR;
			if (strcmp(filter, "Linear Mipmap Linear") == 0) return TextureFilter::LINEAR_MIPMAP_LINEAR;
			return TextureFilter::NEAREST;
		}
	}
}