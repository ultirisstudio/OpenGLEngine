#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class TerrainComponent : public Component
	{
	private:
		std::string height_map_path;

		std::shared_ptr<OpenGLEngine::Texture> m_NoTexture;
	public:
		std::string m_ModelPath;

		TerrainComponent();

		void SetHeightMap(const std::string& path) { height_map_path = path; }
		std::string GetHeightMap() { return height_map_path; }

		Texture& GetEditorHeightMapTexture();

		void GenerateTerrain();
	};
}