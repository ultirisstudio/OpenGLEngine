#pragma once

#include <QuasarEngine/Resources/Texture2D.h>

namespace QuasarEngine
{
	class DirectXTexture2D : public Texture2D
	{
	public:
		DirectXTexture2D(const std::string& path, const TextureSpecification& specification);
		DirectXTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification);
		~DirectXTexture2D() override;

		void Bind(int index) const override;
		void Unbind() const override;
	};
}
