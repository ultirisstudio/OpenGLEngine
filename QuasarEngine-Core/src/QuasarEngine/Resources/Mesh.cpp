#include "qepch.h"

#include <glm/gtx/matrix_decompose.hpp>

#include <QuasarEngine/Resources/Mesh.h>
#include <QuasarEngine/Renderer/RenderCommand.h>

namespace QuasarEngine
{
	void Mesh::CalculateBoundingBoxSize(std::vector<Vertex> vertices)
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		for (const auto& vertex : vertices)
		{
			if (vertex.position.x < minX)
				minX = vertex.position.x;
			if (vertex.position.y < minY)
				minY = vertex.position.y;
			if (vertex.position.z < minZ)
				minZ = vertex.position.z;

			if (vertex.position.x > maxX)
				maxX = vertex.position.x;
			if (vertex.position.y > maxY)
				maxY = vertex.position.y;
			if (vertex.position.z > maxZ)
				maxZ = vertex.position.z;
		}

		m_boundingBoxSize = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
		m_boundingBoxPosition = glm::vec3(minX, minY, minZ);
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, DrawMode drawMode) :
		m_vao(0),
		m_vbo(0),
		m_ebo(0),
		m_drawMode(drawMode),
		m_vertices(),
		m_indices()
	{
		GenerateMesh(vertices, indices);
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const MaterialSpecification& material, DrawMode drawMode) :
		m_vao(0),
		m_vbo(0),
		m_ebo(0),
		m_drawMode(drawMode),
		m_vertices(),
		m_indices(),
		m_material(material)
	{
		GenerateMesh(vertices, indices);
	}

	Mesh::~Mesh()
	{

	}

	// TODO : put DrawMode variable on draw fonction
	void Mesh::draw() const
	{
		m_vertexArray->Bind();

		uint32_t size = m_vertexBuffer->GetSize();
		uint32_t count = m_vertexArray->GetIndexBuffer()->GetCount();

		if (count == 0)
			RenderCommand::DrawArrays(m_drawMode, size);
		else
			RenderCommand::DrawElements(m_drawMode, count);
	}

	void Mesh::GenerateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		CalculateBoundingBoxSize(vertices);

		m_vertexArray = VertexArray::Create();

		m_vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
		m_vertexBuffer->SetLayout({
			{ ShaderDataType::Vec3, "vPosition"				},
			{ ShaderDataType::Vec3, "vNormal"				},
			{ ShaderDataType::Vec2, "vTextureCoordinates"	}
			});
		m_vertexArray->AddVertexBuffer(m_vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
		m_vertexArray->SetIndexBuffer(indexBuffer);

		m_vertices = std::move(vertices);
		m_indices = std::move(indices);
	}

	bool Mesh::IsVisible(const Math::Frustum& frustum, const glm::mat4& modelMatrix) const
	{
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat oriantation;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(modelMatrix, scale, oriantation, position, skew, perspective);
		for (unsigned i = 0; i < std::size(frustum.planes); i++)
		{
			glm::vec3 positive = position + m_boundingBoxPosition;
			if (frustum.planes[i].a >= 0)
				positive.x += m_boundingBoxSize.x;
			if (frustum.planes[i].b >= 0)
				positive.y += m_boundingBoxSize.y;
			if (frustum.planes[i].c >= 0)
				positive.z += m_boundingBoxSize.z;

			if (positive.x * frustum.planes[i].a + positive.y * frustum.planes[i].b + positive.z * frustum.planes[i].c + frustum.planes[i].d < 0)
				return false;
		}

		return true;
	}
}