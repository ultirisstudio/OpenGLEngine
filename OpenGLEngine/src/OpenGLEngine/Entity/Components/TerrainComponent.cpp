#include "depch.h"
#include "TerrainComponent.h"

#include <glad/glad.h>

#include <OpenGLEngine/Renderer/Renderer.h>
#include <OpenGLEngine/Entity/Components/ModelComponent.h>
#include <OpenGLEngine/Resources/Model.h>

#include <stb_image.h>

namespace OpenGLEngine
{
	TerrainComponent::TerrainComponent()
	{
		m_NoTexture = Texture::CreateTexture("Assets/Textures/3d-modeling.png");

		m_Shader = Shader();
		m_Shader.LoadFromFile("Shaders/gpuheight.vs", "Shaders/gpuheight.fs", "Shaders/gpuheight.tcs", "Shaders/gpuheight.tes");

		glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &m_MaxTessLevel);
		std::cout << "Max available tess level: " << m_MaxTessLevel << std::endl;
	}

	Texture& TerrainComponent::GetEditorHeightMapTexture()
	{
		if (height_map_path != "")
		{
			return *Renderer::m_SceneData.m_ResourceManager.getTexture(height_map_path);
		}

		return *m_NoTexture;
	}

	Shader& TerrainComponent::GetShader()
	{
		return m_Shader;
	}

	void TerrainComponent::GenerateTerrain()
	{
		m_Generated = true;

		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned int format;
		unsigned int internalFormat;
		int width, height, nrChannels;
		unsigned char* data = stbi_load(height_map_path.c_str(), &width, &height, &nrChannels, 0);

		if (nrChannels == 4)
		{
			format = GL_RGBA;
			internalFormat = GL_RGBA;
		}
		else if (nrChannels == 3)
		{
			format = GL_RGB;
			internalFormat = GL_RGB;
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			m_Shader.setUniform("heightMap", 0);
			std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

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