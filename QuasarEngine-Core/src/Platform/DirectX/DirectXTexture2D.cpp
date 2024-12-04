#include "qepch.h"
#include "DirectXTexture2D.h"

namespace QuasarEngine
{
	DirectXTexture2D::DirectXTexture2D(const std::string& path, const TextureSpecification& specification) : Texture2D(specification)
	{
		
	}

	DirectXTexture2D::DirectXTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification) : Texture2D(specification)
	{
		
	}


	DirectXTexture2D::~DirectXTexture2D()
	{
		
	}

	void DirectXTexture2D::Bind() const
	{
		
	}

	void DirectXTexture2D::Unbind() const
	{
		
	}
}