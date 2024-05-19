#include "depch.h"
#include "MeshComponent.h"

namespace OpenGLEngine
{
	MeshComponent::MeshComponent() : m_Mesh(nullptr), m_Name("")
	{

	}

	MeshComponent::MeshComponent(std::string name, Mesh* mesh, std::string modelPath) : m_Mesh(mesh), m_Name(name), m_ModelPath(modelPath)
	{
		
	}

	void MeshComponent::GenerateMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, DrawMode drawMode)
	{
		m_Mesh = new Mesh(vertices, indices, drawMode);
	}
}