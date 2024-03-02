#include "depch.h"
#include "PerlinManager.h"

#include <OpenGLEngine/Tools/UUID.h>
#include <OpenGLEngine/Tools/PerlinNoise.h>

namespace OpenGLEngine
{
	struct PerlinInfos
	{
		siv::PerlinNoise::seed_type seed;
		siv::PerlinNoise perlin;
	};

	struct PerlinManagerData
	{
		std::unordered_map<uint64_t, PerlinInfos> m_PerlinMap;
	};

	static PerlinManagerData* s_Data = nullptr;

	void PerlinManager::Init()
	{
		s_Data = new PerlinManagerData();
	}

	uint64_t PerlinManager::AddPerlinNoise()
	{
		uint64_t uuid = UUID();
		s_Data->m_PerlinMap[uuid] = { 8173561, siv::PerlinNoise() };
		return uuid;
	}

	float PerlinManager::GetPerlinNoise(uint64_t uuid, float x, float z)
	{
		return s_Data->m_PerlinMap[uuid].perlin.octave2D(x, z, 2);
	}
}