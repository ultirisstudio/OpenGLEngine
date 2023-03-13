#pragma once

#include <OpenGLEngine/Resources/Vertex.h>

#include <vector>

namespace OpenGLEngine
{
	class Mesh
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
	};
}