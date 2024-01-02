#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Shader/Shader.h>

namespace OpenGLEngine
{
	class TerrainComponent : public Component
	{
	private:
		std::shared_ptr<Texture> m_NoTexture;
		Shader m_Shader;

		std::string height_map_path;
		std::string m_ModelPath;

		bool m_Generated = false;

		int m_MaxTessLevel;
		unsigned rez = 20;

		unsigned int terrainVAO;
	public:
		TerrainComponent();

		bool m_PolygonMode = false;

		void SetHeightMap(const std::string& path) { height_map_path = path; }
		std::string GetHeightMap() { return height_map_path; }

		Texture& GetEditorHeightMapTexture();

		Shader& GetShader();

		bool IsGenerated() { return m_Generated; }

		void GenerateTerrain();
		void Draw();
	};
}