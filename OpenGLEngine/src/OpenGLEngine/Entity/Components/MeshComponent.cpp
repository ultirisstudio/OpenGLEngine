#include "depch.h"
#include "MeshComponent.h"

namespace OpenGLEngine
{
	MeshComponent::MeshComponent() : m_Mesh(nullptr), m_Name("")
	{

	}

	MeshComponent::MeshComponent(std::string name, Mesh* mesh) : m_Mesh(mesh), m_Name(name)
	{
		
	}
}