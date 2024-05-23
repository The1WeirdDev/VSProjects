#pragma once

#include <array>
#include <vector>
#include <functional>

#include "OGLEngine/OGLEngine.h"

#ifndef OGLENGINE_KEY_MAP_SIZE
#define OGLENGINE_KEY_MAP_SIZE 400
#endif

#ifndef OGLENGINE_MOUSE_BUTTON_MAP_SIZE
#define OGLENGINE_MOUSE_BUTTON_MAP_SIZE 8
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

		static OGLENGINE_API bool IsMouseButtonDown(int button);
		static OGLENGINE_API bool IsMouseButtonPressed(int button);

		static OGLENGINE_API std::function<void(int key, bool is_key_down)> on_key_callback;
		static OGLENGINE_API std::function<void(int button, bool is_key_down)> on_mouse_button_callback;
	public:
		static std::array<unsigned char, OGLENGINE_KEY_MAP_SIZE> keys;
		static std::array<unsigned char, OGLENGINE_MOUSE_BUTTON_MAP_SIZE> buttons;
		static std::vector<int> keys_to_update;
		static std::vector<int> mouse_buttons_to_update;

		static double mouse_pos_x, mouse_pos_y;
		static double last_mouse_pos_x, last_mouse_pos_y;
		static double delta_x, delta_y;
	};
}