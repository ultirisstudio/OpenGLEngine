#include "depch.h"
#include <DuckEngine/Tools/Log.h>

bool DuckEngine::Log::firstLog = true;

void DuckEngine::Log::beginLog(const std::string& prefix)
{
	if (firstLog)
	{
		std::cout << prefix;
		firstLog = false;
	}
}

void DuckEngine::Log::endLog()
{
	std::cout << std::endl;
	firstLog = true;
}