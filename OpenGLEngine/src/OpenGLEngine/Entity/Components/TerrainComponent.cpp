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
		m_NoTexture = OpenGLEngine::Texture::CreateTexture("Assets/Textures/3d-modeling.png");
	}

	Texture& TerrainComponent::GetEditorHeightMapTexture()
	{
		if (height_map_path != "")
		{
			return *Renderer::m_SceneData.m_ResourceManager.getTexture(height_map_path);
		}

		return *m_NoTexture;
	}

	void TerrainComponent::GenerateTerrain()
	{
		//if (m_Generated)
			//return;

		m_Generated = true;

		int width, height, nChannels;
		unsigned char* data = stbi_load(height_map_path.c_str(), &width, &height, &nChannels, 0);

		float yScale, yShift;
		int rez;
		unsigned bytePerPixel;

		yScale = 64.0f / 256.0f;
		yShift = 16.0f;
		rez = 1;
		bytePerPixel = nChannels;

		std::vector<Vertex> vertices;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				Vertex vertex;

				unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
				unsigned char y = pixelOffset[0];

				vertex.position.x = -height / 2.0f + i;
				vertex.position.y = (int)y * yScale - yShift;
				vertex.position.z = -width / 2.0f + j;

				vertex.normal.x = 0;
				vertex.normal.y = 1;
				vertex.normal.z = 0;

				vertex.texCoord.x = (float)j / ((float)width - 1);
				vertex.texCoord.y = (float)i / ((float)height - 1);
				
				vertices.push_back(vertex);
			}
		}

		std::cout << "Loaded " << vertices.size() << " vertices" << std::endl;

		stbi_image_free(data);

		std::vector<unsigned> indices;
		for (unsigned i = 0; i < height - 1; i++)
		{
			for (unsigned j = 0; j < width; j++)
			{
				for (unsigned k = 0; k < 2; k++)
				{
					indices.push_back(j + width * (i + k));
				}
			}
		}

		std::cout << "Loaded " << indices.size() << " indices" << std::endl;

		numStrips = (height - 1) / rez;
		numTrisPerStrip = (width / rez) * 2 - 2;
		std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
		std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;

		unsigned int terrainVBO, terrainEBO;
		glGenVertexArrays(1, &terrainVAO);
		glGenBuffers(1, &terrainVBO);
		glGenBuffers(1, &terrainEBO);

		glBindVertexArray(terrainVAO);

		glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));

		//entity->GetComponent<ModelComponent>().SetModel(Model::CreateModel(vertices, indices));
	}
	void TerrainComponent::Draw()
	{
		if (!m_Generated)
			return;

		glBindVertexArray(terrainVAO);

		for (unsigned strip = 0; strip < numStrips; strip++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, numTrisPerStrip + 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip));
		}
	}
}