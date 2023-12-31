#include "depch.h"
#include "TerrainComponent.h"

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
		int width, height, nChannels;
		unsigned char* data = stbi_load("resources/heightmaps/iceland_heightmap.png", &width, &height, &nChannels, 0);

		std::vector<Vertex> vertices;
		//std::vector<float> vertices;
		float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				Vertex vertex;

				// retrieve texel for (i,j) tex coord
				unsigned char* texel = data + (j + width * i) * nChannels;
				// raw height at coordinate
				unsigned char y = texel[0];

				// vertex
				vertex.position.x = -height / 2.0f + i;
				vertex.position.y = (int)y * yScale - yShift;
				vertex.position.z = -width / 2.0f + j;

				// normal
				// retrieve texel for (i-1,j) tex coord
				unsigned char* texel1 = data + (j + width * (i - 1)) * nChannels;
				// raw height at coordinate
				unsigned char y1 = texel1[0];
				// retrieve texel for (i,j-1) tex coord
				unsigned char* texel2 = data + ((j - 1) + width * i) * nChannels;
				// raw height at coordinate
				unsigned char y2 = texel2[0];
				// retrieve texel for (i+1,j) tex coord
				unsigned char* texel3 = data + (j + width * (i + 1)) * nChannels;
				// raw height at coordinate
				unsigned char y3 = texel3[0];
				// retrieve texel for (i,j+1) tex coord
				unsigned char* texel4 = data + ((j + 1) + width * i) * nChannels;
				// raw height at coordinate
				unsigned char y4 = texel4[0];

				// normal
				glm::vec3 normal = glm::normalize(glm::vec3(y1 - y3, 2.0f, y2 - y4));

				vertex.normal.x = normal.x;
				vertex.normal.y = normal.y;
				vertex.normal.z = normal.z;

				// texture coordinates
				vertex.texCoord.x = (float)j / (float)width;
				vertex.texCoord.y = (float)i / (float)height;
				
				vertices.push_back(vertex);
			}
		}

		stbi_image_free(data);

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < height - 1; i++)       // for each row a.k.a. each strip
		{
			for (unsigned int j = 0; j < width; j++)      // for each column
			{
				for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
				{
					indices.push_back(j + width * (i + k));
				}
			}
		}

		entity->GetComponent<ModelComponent>().SetModel(Model::CreateModel(vertices, indices));
	}
}