#pragma once

#include "QuasarEngine/Core/PlatformDetection.h"

#include <memory>

#ifdef PLATFORM_WINDOWS
#if DYNAMIC_LINK
	#ifdef BUILD_DLL
		#define DUCK_API __declspec(dllexport)
	#else
		#define DUCK_API __declspec(dllimport)
	#endif
#else
	#define DUCK_API
#endif
#else
	#ifdef PLATFORM_LINUX
		#if DYNAMIC_LINK
			#ifdef BUILD_DLL
				#define DUCK_API __declspec(dllexport)
			#else
				#define DUCK_API __declspec(dllimport)
			#endif
		#else
			#define DUCK_API
		#endif
	#else
		#error QuasarEngine not support this OS !
	#endif
#endif