#include "depch.h"
#include <OpenGLEngine/Tools/Log.h>

bool OpenGLEngine::Log::firstLog = true;

void OpenGLEngine::Log::beginLog(const std::string& prefix)
{
	if (firstLog)
	{
		std::cout << prefix;
		firstLog = false;
	}
}

void OpenGLEngine::Log::endLog()
{
	std::cout << std::endl;
	firstLog = true;
}