#pragma once

#include <QuasarEngine/Resources/Texture2D.h>

namespace QuasarEngine
{
	class VulkanTexture2D : public Texture2D
	{
	public:
		VulkanTexture2D(const std::string& path, const TextureSpecification& specification);
		VulkanTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification);
		~VulkanTexture2D() override;

		void Bind(int index) const override;
		void Unbind() const override;
	};
}
