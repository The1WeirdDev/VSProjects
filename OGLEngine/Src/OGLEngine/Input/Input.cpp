#include "Input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <OGLEngine/Display/Display.h>

std::array<unsigned char, OGLENGINE_KEY_MAP_SIZE> Input::keys;
void Input::Init() {
	keys.fill(0);
	
}

void Input::Update() {

}
bool Input::IsKeyDown(int key) {
	return keys[key] > 0;
}

#ifdef OGLENGINE_STATIC
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key < 0 || key > OGLENGINE_KEY_MAP_SIZE)return;

	if (action == GLFW_PRESS) {
		keys[key] = 2;
	}else if(action == GLFW_RELEASE) {
		keys[key] = 0;
	}
}
#endif