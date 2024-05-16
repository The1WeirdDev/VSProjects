// MathLibrary.h - Contains declarations of math functions
#pragma once

/*
INTENDED PLATFORMS
WINDOWS
ANDROID
*/

//Its more of a framework

#ifndef OGLENGINE_STATIC
	#ifdef OGLENGINE_EXPORT
	#define OGLENGINE_API __declspec(dllexport)
	#else
	#define OGLENGINE_API __declspec(dllimport)
	#endif
#else
	#define OGLENGINE_API
#endif