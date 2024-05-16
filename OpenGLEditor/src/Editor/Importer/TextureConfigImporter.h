#pragma once

#include <filesystem>
#include <OpenGLEngine/Resources/Texture.h>

namespace OpenGLEngine
{
	class TextureConfigImporter
	{
	public:
		static TextureSpecification ImportTextureConfig(std::filesystem::path path);
		static void ExportTextureConfig(std::filesystem::path path, const TextureSpecification& specification);
	};
}