#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <OpenGLEngine/Asset/Asset.h>

namespace OpenGLEngine
{
	enum class TextureFormat
	{
		RED = 0,
		RGB,
		RGBA,
		SRGB,
		SRGBA,
	};

	enum class TextureWrap
	{
		REPEAT = 0,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class TextureFilter
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
}