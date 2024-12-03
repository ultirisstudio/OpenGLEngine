#pragma once

#include <QuasarEngine/Entity/Component.h>
#include <QuasarEngine/Resources/Mesh.h>

namespace QuasarEngine
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

		void ClearMesh();

		//void GenerateMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, DrawMode drawMode = DrawMode::TRIANGLES);
		void GenerateMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, std::optional<BufferLayout> layout = {}, DrawMode drawMode = DrawMode::TRIANGLES);
	};
}