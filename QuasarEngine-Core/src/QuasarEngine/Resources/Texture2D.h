#pragma once

#include "Texture.h"

namespace QuasarEngine
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& path, const TextureSpecification& specification);
		Texture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification);
		~Texture2D() = default;

		static unsigned char* LoadDataFromPath(const std::string& path, size_t* file_size);

		static std::shared_ptr<Texture2D> CreateTexture2D(const std::string& path, const TextureSpecification& specification);
		static std::shared_ptr<Texture2D> CreateTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification);

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}