#pragma once

#include <vector>
#include <string>

namespace DuckEngine
{
	class CubeMap
	{
	public:
		CubeMap();

		void Load(std::array<std::string, 6> paths);
		void ActiveTexture();
		void BeginDrawModel();
		void EndDrawModel();
		void Bind();

		unsigned int GetID() { return m_ID; }
	private:
		unsigned int m_ID;
	};
}