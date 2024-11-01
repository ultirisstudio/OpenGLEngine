#include "qepch.h"
#include "TerrainComponent.h"

#include <QuasarEngine/Renderer/Renderer.h>
#include <QuasarEngine/Resources/Model.h>

#include <stb_image.h>

namespace QuasarEngine
{
	TerrainComponent::TerrainComponent() : terrainVAO(0)
	{
		m_Shader = std::make_shared<Shader>();
		m_Shader->LoadFromFile("Shaders/gpuheight.vs", "Shaders/gpuheight.fs", "Shaders/gpuheight.tcs", "Shaders/gpuheight.tes");

		std::cout << "Max available tess level: " << m_MaxTessLevel << std::endl;
	}

	Shader& TerrainComponent::GetShader()
	{
		return *m_Shader;
	}

	void TerrainComponent::GenerateTerrain()
	{
		m_Generated = true;

		TextureSpecification spec;
		spec.min_filter_param = TextureFilter::LINEAR_MIPMAP_LINEAR;
		spec.mag_filter_param = TextureFilter::LINEAR;
		m_HeightMapTexture = std::make_shared<Texture>(m_HeightMapPath, spec);

		int width, height;
		width = m_HeightMapTexture->GetSpecification().width;
		height = m_HeightMapTexture->GetSpecification().height;

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
	}
	void TerrainComponent::Draw()
	{
		if (!m_Generated)
			return;
	}
}