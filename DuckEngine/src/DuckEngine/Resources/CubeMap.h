#pragma once

#include <vector>
#include <string>

namespace DuckEngine
{
	class CubeMap
	{
	public:
		CubeMap();

		void Load(std::vector<std::string> paths);
		void Bind();

		unsigned int GetID() { return m_ID; }
	private:
		unsigned int m_ID;
	};
}