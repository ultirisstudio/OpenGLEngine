#pragma once

#include "DuckEngine/Core/Core.h"

#ifdef PLATFORM_WINDOWS

extern DuckEngine::Application* DuckEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = DuckEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif