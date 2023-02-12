#pragma once

#ifdef SG_STATIC
	#define SG_API
#else
	#if defined(_WIN32)
		#ifdef SG_EXPORT
			#define SG_API __declspec(dllexport)
		#else
			#define SG_API __declspec(dllimport)
		#endif
	#elif defined(__unix)
		#ifdef SG_EXPORT
			#define SG_API __attribute__((visibility("default")))
		#else
			#define SG_API
		#endif
	#endif
#endif