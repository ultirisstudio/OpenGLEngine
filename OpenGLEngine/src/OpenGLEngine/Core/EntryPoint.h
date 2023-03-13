#pragma once

#include "OpenGLEngine/Core/Core.h"

#ifdef PLATFORM_WINDOWS

extern OpenGLEngine::Application* OpenGLEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = OpenGLEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif