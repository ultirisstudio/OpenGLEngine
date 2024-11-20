#pragma once

#include <vector>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <QuasarEngine/Resources/Texture.h>
#include <QuasarEngine/Asset/AssetHeader.h>

namespace QuasarEngine
{
	class TextureImporter
	{
	public:
		static void exportTexture(const std::string& path, const std::string& out)
		{
			size_t size;
			unsigned char* data = Texture::LoadDataFromPath(path, &size);

			TextureSpecification spec;
			std::shared_ptr<Texture> texture = Texture::CreateTexture(path, spec);

			std::ofstream file(out, std::ios::binary);

			AssetHeader assetHeader = {
				0xDEADBEEF,
				sizeof(AssetHeader) + sizeof(TextureSpecification) + size,
				"Texture"
			};
			file.write(reinterpret_cast<const char*>(&assetHeader), sizeof(assetHeader));

			file.write(reinterpret_cast<const char*>(&texture->GetSpecification()), sizeof(TextureSpecification));

			file.write(reinterpret_cast<const char*>(data), size);

			file.close();

			delete texture.get();
		}

		static std::shared_ptr<Texture> importTexture(const std::string& path) {
			unsigned char* data;

			std::ifstream file(path, std::ios::binary);

			AssetHeader assetHeader;
			file.read(reinterpret_cast<char*>(&assetHeader), sizeof(assetHeader));

			TextureSpecification spec;
			file.read(reinterpret_cast<char*>(&spec), sizeof(TextureSpecification));

			size_t size = spec.width * spec.height * spec.channels;
			data = new unsigned char[size];

			file.read(reinterpret_cast<char*>(data), size);

			file.close();

			std::shared_ptr<Texture> texture = Texture::CreateTexture(data, size, spec);

			return texture;
		}
	};
}