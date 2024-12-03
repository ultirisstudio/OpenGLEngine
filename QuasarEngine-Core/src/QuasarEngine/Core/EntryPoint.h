#pragma once

#include "QuasarEngine/Core/Core.h"

extern QuasarEngine::Application* QuasarEngine::CreateApplication(ApplicationCommandLineArgs args);

#ifdef PLATFORM_WINDOWS

/*#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	char** cstr = new char* [argc];

	for (int i = 0; i < argc; ++i) {
		int len = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);
		cstr[i] = new char[len];
		WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, cstr[i], len, NULL, NULL);
	}

	auto app = QuasarEngine::CreateApplication({ argc, cstr });

	LocalFree(argv);

	app->Run();

	delete app;

	return 0;
}*/

int main(int argc, char** argv)
{
	auto app = QuasarEngine::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#elif

int main(int argc, char** argv)
{
	auto app = QuasarEngine::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#endif