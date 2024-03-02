#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class MeshComponent : public Component
	{
	private:
		Mesh* m_Mesh;
		std::string m_Name;
	public:
		MeshComponent();
		MeshComponent(std::string name, Mesh* mesh);

		Mesh& GetMesh() { return *m_Mesh; }
		std::string GetName() { return m_Name; }

		bool HasMesh() { return !(m_Mesh == nullptr); }

		void GenerateMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) { m_Mesh = new Mesh(vertices, indices); }
	};
}