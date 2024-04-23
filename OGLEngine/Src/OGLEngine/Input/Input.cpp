#include "Input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <OGLEngine/Display/Display.h>

std::array<unsigned char, 400> Input::keys;
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
	printf("KEY CALLBACK %d %d %d %d\n", key, scancode, action, mods);
}
#endif