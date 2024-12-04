#pragma once

#include <QuasarEngine/Resources/Texture2D.h>

namespace QuasarEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, const TextureSpecification& specification);
		OpenGLTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification);
		~OpenGLTexture2D() override;

		void Bind() const override;
		void Unbind() const override;
	};
}