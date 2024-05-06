#pragma once

#include <vector>

#include <OpenGLEngine/Resources/Vertex.h>
#include <OpenGLEngine/Asset/Asset.h>

namespace OpenGLEngine
{
	class Mesh : public Asset
	{
	private:
		unsigned int m_vao;
		unsigned int m_vbo;
		unsigned int m_ebo;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		~Mesh();

		void draw() const;

		const size_t& GetVerticesCount() { return m_vertices.size(); }
		const size_t& GetIndicesCount() { return m_indices.size(); }

		const std::vector<Vertex>& GetVertices() const { return m_vertices; }
		const std::vector<unsigned int>& GetIndices() const { return m_indices; }

		static AssetType GetStaticType() { return AssetType::MESH; }
		virtual AssetType GetType() override { return GetStaticType(); }
	};
}