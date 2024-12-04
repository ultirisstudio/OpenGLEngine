#include "qepch.h"
#include "Texture2D.h"

#include <fstream>

#include <QuasarEngine/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLTexture2D.h>
#include <Platform/DirectX/DirectXTexture2D.h>

namespace QuasarEngine
{
	std::shared_ptr<Texture2D> Texture2D::CreateTexture2D(const std::string& path, const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path, specification);
		case RendererAPI::API::DirectX:  return std::make_shared<DirectXTexture2D>(path, specification);
		}

		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::CreateTexture2D(unsigned char* image_data, size_t size, const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(image_data, size, specification);
		case RendererAPI::API::DirectX:  return std::make_shared<DirectXTexture2D>(image_data, size, specification);
		}

		return nullptr;
	}

	unsigned char* readFile(const std::string& filename, size_t* file_size) {
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open()) {
			return nullptr;
		}

		file.seekg(0, std::ios::end);
		std::streampos size = file.tellg();
		file.seekg(0, std::ios::beg);

		*file_size = size;

		unsigned char* data = new unsigned char[size];

		file.read((char*)data, size);
		file.close();
		return data;
	}

	Texture2D::Texture2D(const TextureSpecification& spec) : Texture(spec)
	{

	}

	unsigned char* Texture2D::LoadDataFromPath(const std::string& path, size_t* file_size)
	{
		return readFile(path, file_size);
	}
}