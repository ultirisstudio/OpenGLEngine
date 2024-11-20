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
			std::unique_ptr<unsigned char[]> data(Texture::LoadDataFromPath(path, &size));

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

			file.write(reinterpret_cast<const char*>(data.get()), size);

			file.close();
		}

		static void updateTexture(const std::string& path, const TextureSpecification& spec)
		{
			std::ifstream in_file(path, std::ios::binary);

			AssetHeader assetHeader;
			in_file.read(reinterpret_cast<char*>(&assetHeader), sizeof(assetHeader));

			TextureSpecification local_spec;
			in_file.read(reinterpret_cast<char*>(&local_spec), sizeof(TextureSpecification));

			size_t size = spec.width * spec.height * spec.channels;
			unsigned char* data = new unsigned char[size];

			in_file.read(reinterpret_cast<char*>(data), size);

			in_file.close();

			std::ofstream out_file(path, std::ios::binary);
			if (!out_file.is_open()) {
				return;
			}

			out_file.write(reinterpret_cast<const char*>(&assetHeader), sizeof(assetHeader));
			out_file.write(reinterpret_cast<const char*>(&spec), sizeof(TextureSpecification));
			out_file.write(reinterpret_cast<const char*>(data), size);

			out_file.close();
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