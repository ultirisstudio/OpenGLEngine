#pragma once

#include <OpenGLEngine/Entity/Component.h>
#include <OpenGLEngine/Shader/Shader.h>

namespace OpenGLEngine
{
	class TerrainComponent : public Component
	{
	private:
		std::shared_ptr<Texture> m_HeightMapTexture;
		std::shared_ptr<Shader> m_Shader;

		std::string m_HeightMapPath;

		bool m_Generated = false;

		int m_MaxTessLevel;
		unsigned rez = 20;

		unsigned int terrainVAO;
	public:
		TerrainComponent();

		bool m_PolygonMode = false;

		void SetHeightMap(const std::string& path) { m_HeightMapPath = path; }

		std::string GetHeightMapPath() { return m_HeightMapPath; }
		Texture& GetHeightMapTexture() { return *m_HeightMapTexture; }

		Shader& GetShader();

		bool IsGenerated() { return m_Generated; }

		void GenerateTerrain();
		void Draw();
	};
}