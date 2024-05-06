#pragma once

#include <vector>

#include <OpenGLEngine/Resources/Debug/DebugVertex.h>

namespace OpenGLEngine
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