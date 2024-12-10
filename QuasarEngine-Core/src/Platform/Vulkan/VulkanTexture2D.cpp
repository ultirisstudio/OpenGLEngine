#include "qepch.h"
#include "VulkanTexture2D.h"

namespace QuasarEngine
{
	VulkanTexture2D::VulkanTexture2D(const std::string& path, const TextureSpecification& specification) : Texture2D(specification)
	{
		
	}

	VulkanTexture2D::VulkanTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification) : Texture2D(specification)
	{
		
	}


	VulkanTexture2D::~VulkanTexture2D()
	{
		
	}

	void VulkanTexture2D::Bind(int index) const
	{
		
	}

	void VulkanTexture2D::Unbind() const
	{
		
	}
}