#pragma once

#include <vector>

#include <QuasarEngine/Resources/Debug/DebugVertex.h>

namespace QuasarEngine
{
	class DebugLineMesh
	{
	private:
		unsigned int m_vao;
		unsigned int m_vbo;

		std::vector<DebugLineVertex> m_vertices;

	public:
		DebugLineMesh(std::vector<DebugLineVertex>& vertices);
		~DebugLineMesh();

		void draw() const;
	};
}