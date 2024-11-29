#include "qepch.h"
#include "MeshComponent.h"

namespace QuasarEngine
{
	MeshComponent::MeshComponent() : m_Mesh(nullptr), m_Name("")
	{

	}

	MeshComponent::MeshComponent(std::string name, Mesh* mesh, std::string modelPath) : m_Mesh(mesh), m_Name(name), m_ModelPath(modelPath)
	{
		
	}

	void MeshComponent::ClearMesh()
	{
		m_Mesh->Clear();
	}

	void MeshComponent::GenerateMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, std::optional<BufferLayout> layout, DrawMode drawMode)
	{
		m_Mesh = new Mesh(vertices, indices, layout, drawMode);
	}

	/*void MeshComponent::GenerateMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, DrawMode drawMode)
	{
		m_Mesh = new Mesh(vertices, indices, drawMode);
	}*/
}