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
		~TextureArray() = default;

		static std::shared_ptr<TextureArray> CreateTextureArray(const std::vector<std::string>& paths, const TextureSpecification& specification);

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}