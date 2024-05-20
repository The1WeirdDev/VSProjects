#pragma once

#include <array>
#include <vector>
#include <functional>

#include "OGLEngine/OGLEngine.h"

#ifndef OGLENGINE_KEY_MAP_SIZE
#define OGLENGINE_KEY_MAP_SIZE 400
#endif

namespace T1WD {
	extern "C" OGLENGINE_API class Input {
	public:
		static OGLENGINE_API void Init();

		//Call this update after you update your game
		//Doing it this way will let you check for IsKeyPressed
		static OGLENGINE_API void Update();

		static OGLENGINE_API bool IsKeyDown(int key);
		static OGLENGINE_API bool IsKeyPressed(int key);

		//static OGLENGINE_API void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static OGLENGINE_API std::function<void(int key, bool is_key_down)> on_key_callback;
	public:
		static std::array<unsigned char, 400> keys;
		static std::vector<int> keys_to_update;
	};
}