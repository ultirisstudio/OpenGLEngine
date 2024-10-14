#pragma once

#include "qepch.h"
#include "UUID.h"

namespace QuasarEngine {
    static std::random_device                       s_RandomDevice;
    static std::mt19937_64                          s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint32_t>  s_UniformDistribution;

	UUID::UUID() : m_UUID(s_UniformDistribution(s_Engine))
	{
		
	}

	UUID::UUID(uint32_t uuid)
		: m_UUID(uuid)
	{
	}
}