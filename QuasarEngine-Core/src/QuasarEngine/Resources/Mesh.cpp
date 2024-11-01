#include "qepch.h"
#include <QuasarEngine/Resources/Mesh.h>

#include <glm/gtx/matrix_decompose.hpp>

namespace QuasarEngine
{
	namespace Utils
	{
		/*static GLenum DrawModeToGLenum(DrawMode drawMode)
		{
			switch (drawMode)
			{
			case DrawMode::TRIANGLES: return GL_TRIANGLES;
			case DrawMode::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
			case DrawMode::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
			case DrawMode::LINES: return GL_LINES;
			case DrawMode::LINE_STRIP: return GL_LINE_STRIP;
			case DrawMode::LINE_LOOP: return GL_LINE_LOOP;
			case DrawMode::POINTS: return GL_POINTS;
			}
			return GL_TRIANGLES;
		}*/
	}

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
		m_vertices(std::move(vertices)),
		m_indices(std::move(indices))
	{
		GenerateMesh(m_vertices, m_indices);
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const MaterialSpecification& material, DrawMode drawMode) :
		m_vao(0),
		m_vbo(0),
		m_ebo(0),
		m_drawMode(drawMode),
		m_vertices(std::move(vertices)),
		m_indices(std::move(indices)),
		m_material(material)
	{
		GenerateMesh(m_vertices, m_indices);
	}

	Mesh::~Mesh()
	{
		//glDeleteVertexArrays(1, &m_vao);
		//glDeleteBuffers(1, &m_vbo);
		//glDeleteBuffers(1, &m_ebo);
	}

	// TODO : put DrawMode variable on draw fonction
	void Mesh::draw() const
	{
		/*glBindVertexArray(m_vao);

		if (m_indices.empty())
			glDrawArrays(Utils::DrawModeToGLenum(m_drawMode), 0, static_cast<GLsizei>(m_vertices.size()));
		else
			glDrawElements(Utils::DrawModeToGLenum(m_drawMode), static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);*/
	}

	void Mesh::GenerateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		CalculateBoundingBoxSize(vertices);

		/*glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));*/
	}

	bool Mesh::IsVisible(const Math::Frustum& frustum, const glm::mat4& modelMatrix) const
	{
		glm::vec3 position;
		glm::decompose(modelMatrix, glm::vec3(), glm::quat(), position, glm::vec3(), glm::vec4());
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