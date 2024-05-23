#include "Input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <OGLEngine/Display/Display.h>

using namespace T1WD;
std::function<void(int key, bool is_key_down)> Input::on_key_callback = nullptr;
std::function<void(int button, bool is_key_down)> Input::on_mouse_button_callback = nullptr;
std::array<unsigned char, OGLENGINE_KEY_MAP_SIZE> Input::keys;
std::array<unsigned char, OGLENGINE_MOUSE_BUTTON_MAP_SIZE> Input::buttons;
std::vector<int> Input::keys_to_update;
std::vector<int> Input::mouse_buttons_to_update;

double Input::mouse_pos_x = 0, Input::mouse_pos_y = 0;
double Input::last_mouse_pos_x = 0, Input::last_mouse_pos_y = 0;
double Input::delta_x = 0, Input::delta_y = 0;

void Input::Init() {
	keys.fill(0);
	keys_to_update.resize(0);
}

void Input::Update() {
	Input::delta_x = 0;
	Input::delta_y = 0;
	for (int i = 0; i < keys_to_update.size(); i++) {
		if (keys[keys_to_update[i]] > 1) {
			keys[keys_to_update[i]] = 1;
		}
	}

	keys_to_update.clear();

	for (int i = 0; i < mouse_buttons_to_update.size(); i++) {
		if (buttons[mouse_buttons_to_update[i]] > 1) {
			buttons[mouse_buttons_to_update[i]] = 1;
		}
	}

	mouse_buttons_to_update.clear();
}
bool Input::IsKeyDown(int key) {
	return keys[key] > 0;
}
bool Input::IsKeyPressed(int key) {
	return keys[key] > 1;
}
bool Input::IsMouseButtonDown(int button) {
	if (button < 0 || button >= OGLENGINE_MOUSE_BUTTON_MAP_SIZE)
		return false;
	return buttons[button] > 0;
}
bool Input::IsMouseButtonPressed(int button) {
	if (button < 0 || button >= OGLENGINE_MOUSE_BUTTON_MAP_SIZE)
		return false;
	return buttons[button] == 2;
}