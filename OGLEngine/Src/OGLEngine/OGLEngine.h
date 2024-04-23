// MathLibrary.h - Contains declarations of math functions
#pragma once

#define OGLENGINE_STATIC
/*
INTENDED PLATFORMS
WINDOWS
ANDROID
*/

#ifndef OGLENGINE_STATIC
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