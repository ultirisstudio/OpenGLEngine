#pragma once

#include <vector>
#include <string>

#include "Texture.h"

namespace QuasarEngine
{
	class TextureArray : public Texture
	{
	public:
		TextureArray(const std::vector<std::string>& paths, const TextureSpecification& specification);
		TextureArray(const std::vector<unsigned char*>& files, const std::vector<size_t>& sizes, const TextureSpecification& specification);
		~TextureArray() = default;

		static std::shared_ptr<TextureArray> CreateTextureArray(const std::vector<std::string>& paths, const TextureSpecification& specification);
		static std::shared_ptr<TextureArray> CreateTextureArray(const std::vector<unsigned char*>& files, const std::vector<size_t>& sizes, const TextureSpecification& specification);

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}