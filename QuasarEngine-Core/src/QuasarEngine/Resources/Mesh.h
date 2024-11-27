#pragma once

#include <vector>

#include <QuasarEngine/Resources/Vertex.h>
#include <QuasarEngine/Asset/Asset.h>
#include <QuasarEngine/Resources/Materials/Material.h>

#include <QuasarEngine/Tools/Math.h>


#include <QuasarEngine/Renderer/VertexArray.h>
#include <QuasarEngine/Renderer/Buffer.h>
#include <QuasarEngine/Renderer/DrawMode.h>

namespace QuasarEngine
{
	class Mesh : public Asset
	{
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<VertexBuffer> m_vertexBuffer;

		DrawMode m_drawMode;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		glm::vec3 m_boundingBoxSize;
		glm::vec3 m_boundingBoxPosition;

		bool m_meshGenerated = false;

		//MaterialSpecification m_material;

		void CalculateBoundingBoxSize(std::vector<Vertex> vertices);
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, DrawMode drawMode = DrawMode::TRIANGLES);
		//Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const MaterialSpecification& material, DrawMode drawMode = DrawMode::TRIANGLES);
		~Mesh();

		void draw() const;

		//const MaterialSpecification& GetMaterial() const { return m_material; }

		void GenerateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

		glm::vec3 GetBoundingBoxSize() const { return m_boundingBoxSize; }

		bool IsVisible(const Math::Frustum& frustum, const glm::mat4& modelMatrix) const;

		bool IsMeshGenerated();

		void Clear();

		const size_t& GetVerticesCount() { return m_vertices.size(); }
		const size_t& GetIndicesCount() { return m_indices.size(); }

		const std::vector<Vertex>& GetVertices() const { return m_vertices; }
		const std::vector<unsigned int>& GetIndices() const { return m_indices; }

		static AssetType GetStaticType() { return AssetType::MESH; }
		virtual AssetType GetType() override { return GetStaticType(); }
	};
}