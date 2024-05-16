#pragma once
#ifdef NETWORKING_STATIC_BUILD
	#define NETWORKING_API
#else
	#ifdef ANDROID_BUILD
		#define fdeclspec
		#ifdef NETWORKING_EXPORTS
			#define NETWORKING_API __declspec(dllexport)
		#else
			#define NETWORKING_API __declspec(dllimport)
		#endif
	#else
		#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
		// Windows Header Files
		#include <windows.h>

		#ifdef NETWORKING_EXPORTS
		#define NETWORKING_API __declspec(dllexport)
		#else
		#define NETWORKING_API __declspec(dllimport)
		#endif
	#endif
#endif