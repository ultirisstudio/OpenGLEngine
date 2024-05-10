#pragma once

#include <OpenGLEngine/Entity/Component.h>

namespace OpenGLEngine
{
	class MeshComponent : public Component
	{
	private:
		Mesh* m_Mesh;
		std::string m_Name;
		std::string m_ModelPath;
	public:
		MeshComponent();
		MeshComponent(std::string name, Mesh* mesh, std::string modelPath);

		Mesh& GetMesh() { return *m_Mesh; }
		std::string GetName() { return m_Name; }
		std::string GetModelPath() { return m_ModelPath; }

		bool HasMesh() { return !(m_Mesh == nullptr); }

		void GenerateMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) { m_Mesh = new Mesh(vertices, indices); }
	};
}