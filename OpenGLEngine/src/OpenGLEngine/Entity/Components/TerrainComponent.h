#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class TerrainComponent : public Component
	{
	private:
		std::shared_ptr<OpenGLEngine::Texture> m_NoTexture;

		std::string height_map_path;
		std::string m_ModelPath;

		bool m_Generated = false;

		int numStrips;
		int numTrisPerStrip;

		unsigned int terrainVAO;
	public:
		TerrainComponent();

		bool m_PolygonMode = false;

		void SetHeightMap(const std::string& path) { height_map_path = path; }
		std::string GetHeightMap() { return height_map_path; }

		Texture& GetEditorHeightMapTexture();

		bool IsGenerated() { return m_Generated; }

		void GenerateTerrain();
		void Draw();
	};
}