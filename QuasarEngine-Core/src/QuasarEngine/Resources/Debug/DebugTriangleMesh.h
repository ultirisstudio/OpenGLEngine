#pragma once

#include <vector>

#include <QuasarEngine/Resources/Debug/DebugVertex.h>

namespace QuasarEngine
{
	class DebugTriangleMesh
	{
	private:
		unsigned int m_vao;
		unsigned int m_vbo;

		std::vector<DebugTriangleVertex> m_vertices;

	public:
		DebugTriangleMesh(std::vector<DebugTriangleVertex>& vertices);
		~DebugTriangleMesh();

		void draw() const;
	};
}