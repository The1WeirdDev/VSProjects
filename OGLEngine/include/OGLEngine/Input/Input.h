#pragma once

#include <array>
#include <vector>

#ifdef OGLENGINE_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include "OGLEngine/OGLEngine.h"

#ifndef OGLENGINE_KEY_MAP_SIZE
#define OGLENGINE_KEY_MAP_SIZE 400
#endif

extern "C" OGLENGINE_API class Input {
public:
	static OGLENGINE_API void Init();
	static OGLENGINE_API void Update();

	static OGLENGINE_API bool IsKeyDown(int key);
	static OGLENGINE_API bool IsKeyPressed(int key);

#ifdef OGLENGINE_STATIC
	static OGLENGINE_API void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
#endif
public:
	static std::array<unsigned char, 400> keys;
	static std::vector<int> keys_to_update;
};