#pragma once

#include "Texture.h"

namespace QuasarEngine
{
	class Texture2D : public Texture
	{
	public:
		Texture2D();
		explicit Texture2D(const TextureSpecification& spec);
		virtual ~Texture2D() = default;

		static unsigned char* LoadDataFromPath(const std::string& path, size_t* file_size);

		void Bind() const override {};
		void Unbind() const override {};

		static std::shared_ptr<Texture2D> CreateTexture2D(const std::string& path, const TextureSpecification& specification);
		static std::shared_ptr<Texture2D> CreateTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification);
	};
}