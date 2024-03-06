#pragma once

#include "OpenGLEngine/Core/Core.h"

#ifdef PLATFORM_WINDOWS

extern OpenGLEngine::Application* OpenGLEngine::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	auto app = OpenGLEngine::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#endif