#include "qepch.h"
#include <QuasarEngine/Resources/Debug/DebugLineMesh.h>

namespace QuasarEngine
{
	DebugLineMesh::DebugLineMesh(std::vector<DebugLineVertex>& vertices) :
		m_vao(0),
		m_vbo(0),
		m_vertices(std::move(vertices))
	{
		/*glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(DebugLineVertex), m_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugLineVertex), (const void*)offsetof(DebugLineVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugLineVertex), (const void*)offsetof(DebugLineVertex, color));*/
	}

	DebugLineMesh::~DebugLineMesh()
	{
		//glDeleteVertexArrays(1, &m_vao);
		//glDeleteBuffers(1, &m_vbo);
	}

	void DebugLineMesh::draw() const
	{
		//glBindVertexArray(m_vao);
		//glDrawArrays(GL_LINE, 0, static_cast<GLsizei>(m_vertices.size()));
	}
}