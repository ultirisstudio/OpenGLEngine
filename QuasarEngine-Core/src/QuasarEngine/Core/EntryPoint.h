#pragma once

#include "QuasarEngine/Core/Core.h"

#ifdef PLATFORM_WINDOWS

extern QuasarEngine::Application* QuasarEngine::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	auto app = QuasarEngine::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#endif