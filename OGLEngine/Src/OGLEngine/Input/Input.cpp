#include "Input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <OGLEngine/Display/Display.h>

std::array<unsigned char, OGLENGINE_KEY_MAP_SIZE> Input::keys;
std::vector<int> Input::keys_to_update;

void Input::Init() {
	keys.fill(0);
	keys_to_update.resize(0);
}

void Input::Update() {
	for (int i = 0; i < keys_to_update.size(); i++) {
		if (keys[keys_to_update[i]] > 1) {
			keys[keys_to_update[i]] = 1;
		}
	}

	keys_to_update.clear();
}
bool Input::IsKeyDown(int key) {
	return keys[key] > 0;
}
bool Input::IsKeyPressed(int key) {
	return keys[key] > 1;
}

#ifdef OGLENGINE_STATIC
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key < 0 || key > OGLENGINE_KEY_MAP_SIZE)return;

	if (action == GLFW_PRESS) {
		keys[key] = 2;
		keys_to_update.push_back(key);

	}else if(action == GLFW_RELEASE) {
		keys[key] = 0;
	}
}
#endif