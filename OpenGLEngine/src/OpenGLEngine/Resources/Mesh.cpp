#include "depch.h"
#include <OpenGLEngine/Resources/Mesh.h>

#include <glad/glad.h>

namespace OpenGLEngine
{
	namespace Utils
	{
		static GLenum DrawModeToGLenum(DrawMode drawMode)
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
		}
	}

	Mesh::Mesh(std::vector<Vertex>&vertices, std::vector<unsigned int>&indices, DrawMode drawMode) :
		m_vao(0),
		m_vbo(0),
		m_ebo(0),
		m_drawMode(drawMode),
		m_vertices(std::move(vertices)),
		m_indices(std::move(indices)),
		m_material({ false, false, "", ""})
	{
		GenerateMesh(vertices, indices);
	}

	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, MeshMaterial material, DrawMode drawMode) :
		m_vao(0),
		m_vbo(0),
		m_ebo(0),
		m_drawMode(drawMode),
		m_vertices(std::move(vertices)),
		m_indices(std::move(indices)),
		m_material(material)
	{
		GenerateMesh(vertices, indices);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}

	// TODO : put DrawMode variable on draw fonction
	void Mesh::draw() const
	{
		glBindVertexArray(m_vao);

		if (m_indices.empty())
			glDrawArrays(Utils::DrawModeToGLenum(m_drawMode), 0, static_cast<GLsizei>(m_vertices.size()));
		else
			glDrawElements(Utils::DrawModeToGLenum(m_drawMode), static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
	}

	void Mesh::GenerateMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		glGenVertexArrays(1, &m_vao);
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
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));
	}
}