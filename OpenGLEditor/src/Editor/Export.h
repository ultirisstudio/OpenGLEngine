#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

namespace OpenGLEngine
{
	class Export
	{
	public:
		struct PakHeader {
			uint32_t magic;
			uint32_t totalSize;
			uint32_t numResources;
		};

		struct PakEntry {
			std::string name;
			uint32_t offset;
			uint32_t size;
			uint8_t flags;
		};

		struct ImageHeader {
			uint32_t signature; // Identifiant du format d'image
			uint32_t width; // Largeur de l'image
			uint32_t height; // Hauteur de l'image
			uint16_t bitsPerPixel; // Nombre de bits par pixel
			uint32_t imageSize; // Taille de l'image en octets
			uint16_t planes; // Nombre de plans de couleur
			uint16_t compression; // Type de compression
			uint32_t imageOffset; // Décalage vers les données d'image
		};

		static bool AddResourceToPak(const std::filesystem::path& resourcePath, std::ofstream& pakFile, bool isCompressed);
		static bool CreatePakFile(const std::vector<std::filesystem::path>& resources, const std::string& pakFilePath, bool isCompressed);

		static std::unordered_map<std::string, std::vector<char>> LoadAllResourcesFromPak(const std::string& pakFilePath);

		static bool GetImageHeader(const std::string& imagePath, ImageHeader& header);
	};
}