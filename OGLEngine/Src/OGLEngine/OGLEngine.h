// MathLibrary.h - Contains declarations of math functions
#pragma once

#define STATIC
/*
INTENDED PLATFORMS
WINDOWS
ANDROID
*/

#ifndef STATIC
	#ifdef OGLENGINE_EXPORT
	#define OGLENGINE_API __declspec(dllexport)
	#else
	#define OGLENGINE_API __declspec(dllimport)
	#endif
#else
	#ifdef OGLENGINE_EXPORT
	#define OGLENGINE_API
	#else
	#define OGLENGINE_API
	#endif
#endif