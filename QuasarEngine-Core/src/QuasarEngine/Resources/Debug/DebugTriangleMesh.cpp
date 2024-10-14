#include "qepch.h"
#include <QuasarEngine/Resources/Debug/DebugTriangleMesh.h>

#include <glad/glad.h>

namespace QuasarEngine
{
	DebugTriangleMesh::DebugTriangleMesh(std::vector<DebugTriangleVertex>& vertices) :
		m_vao(0),
		m_vbo(0),
		m_vertices(std::move(vertices))
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(DebugTriangleVertex), m_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugTriangleVertex), (const void*)offsetof(DebugTriangleVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugTriangleVertex), (const void*)offsetof(DebugTriangleVertex, color));
	}

	DebugTriangleMesh::~DebugTriangleMesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void DebugTriangleMesh::draw() const
	{
		glBindVertexArray(m_vao);

		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
	}
}