#pragma once

#include <unordered_map>

namespace OpenGLEngine
{
	class PerlinManager
	{
	public:
		static void Init();

		static uint64_t AddPerlinNoise();

		static float GetPerlinNoise(uint64_t uuid, float x, float z);
	};
}