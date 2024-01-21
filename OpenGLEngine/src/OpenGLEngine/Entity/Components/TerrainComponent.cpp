#include "depch.h"
#include "TerrainComponent.h"

#include <glad/glad.h>

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Resources/Model.h>

#include <stb_image.h>

namespace OpenGLEngine
{
	TerrainComponent::TerrainComponent() : terrainVAO(0)
	{
		m_Shader = std::make_shared<Shader>();
		m_Shader->LoadFromFile("Shaders/gpuheight.vs", "Shaders/gpuheight.fs", "Shaders/gpuheight.tcs", "Shaders/gpuheight.tes");

		glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &m_MaxTessLevel);
		std::cout << "Max available tess level: " << m_MaxTessLevel << std::endl;
	}

	Shader& TerrainComponent::GetShader()
	{
		return *m_Shader;
	}

	void TerrainComponent::GenerateTerrain()
	{
		m_Generated = true;

		m_HeightMapTexture = std::make_shared<Texture>(m_HeightMapPath, false, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		int width, height;
		width = m_HeightMapTexture->GetWidth();
		height = m_HeightMapTexture->GetHeight();

		std::vector<float> vertices;
		for (unsigned i = 0; i <= rez - 1; i++)
		{
			for (unsigned j = 0; j <= rez - 1; j++)
			{
				vertices.push_back(-width / 2.0f + width * i / (float)rez); // v.x
				vertices.push_back(0.0f); // v.y
				vertices.push_back(-height / 2.0f + height * j / (float)rez); // v.z
				vertices.push_back(i / (float)rez); // u
				vertices.push_back(j / (float)rez); // v

				vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez); // v.x
				vertices.push_back(0.0f); // v.y
				vertices.push_back(-height / 2.0f + height * j / (float)rez); // v.z
				vertices.push_back((i + 1) / (float)rez); // u
				vertices.push_back(j / (float)rez); // v

				vertices.push_back(-width / 2.0f + width * i / (float)rez); // v.x
				vertices.push_back(0.0f); // v.y
				vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
				vertices.push_back(i / (float)rez); // u
				vertices.push_back((j + 1) / (float)rez); // v

				vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez); // v.x
				vertices.push_back(0.0f); // v.y
				vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
				vertices.push_back((i + 1) / (float)rez); // u
				vertices.push_back((j + 1) / (float)rez); // v
			}
		}
		std::cout << "Loaded " << rez * rez << " patches of 4 control points each" << std::endl;
		std::cout << "Processing " << rez * rez * 4 << " vertices in vertex shader" << std::endl;

		unsigned int terrainVBO;
		glGenVertexArrays(1, &terrainVAO);
		glBindVertexArray(terrainVAO);

		glGenBuffers(1, &terrainVBO);
		glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
	}
	void TerrainComponent::Draw()
	{
		if (!m_Generated)
			return;

		glBindVertexArray(terrainVAO);
		glDrawArrays(GL_PATCHES, 0, 4 * rez * rez);
	}
}