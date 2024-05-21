#include "Display.h"

#include <stdio.h>
#include <OGLEngine/LibraryManager.h>

#include <OGLEngine/Input/Input.h>
using namespace T1WD;

GLFWwindow* Display::window = nullptr;
int Display::width = 0;
int Display::height = 0;
float Display::aspect_ratio = 1.0f;
float Display::inverse_aspect_ratio = 1.0f;

std::function<void(int, int)> Display::window_resize_callback;

void Display::Create(int width, int height, const char* title){
	Display::window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwMakeContextCurrent(window);
	LibraryManager::InitializeGLEW();

	glfwGetWindowSize(window, &width, &height);
	AddWindowCallbacks();

	Display::width = width;
	Display::height = height;
	aspect_ratio = (float)width / (float)height;
	inverse_aspect_ratio = (float)height / (float)width;

	glViewport(0, 0, width, height);
}

void Display::AddWindowCallbacks() {
	glfwSetWindowSizeCallback(window, Display::OnWindowResize);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key < 0 || key > OGLENGINE_KEY_MAP_SIZE)return;

		if (action != GLFW_REPEAT) {
			if (action == GLFW_PRESS) {
				Input::keys[key] = 2;
				Input::keys_to_update.push_back(key);

			}
			else if (action == GLFW_RELEASE) {
				Input::keys[key] = 0;
			}

			if(Input::on_key_callback)
				Input::on_key_callback(key, action == GLFW_PRESS);
		}
	});
}
void Display::Destroy() {
	glfwDestroyWindow(window);
	Display::window_resize_callback = nullptr;
}

void Display::CenterWindow() {
	const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (video_mode->width - width) / 2, (video_mode->height - height) / 2);
}
void Display::SetTitle(const char* title) {
	glfwSetWindowTitle(window, title);
}
void Display::SetBackgroundColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void Display::SetSwapInterval(int interval) {
	glfwSwapInterval(interval);
}
void Display::PollEvents() {
	glfwPollEvents();
}
void Display::SwapBuffers() {
	glfwSwapBuffers(window);
}
void Display::ClearColors() {
	glClear(GL_COLOR_BUFFER_BIT);
}
void Display::ClearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}
bool Display::ShouldUpdateWindow() { return glfwWindowShouldClose(window) == false; }

void Display::OnWindowResize(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	Display::width = width;
	Display::height = height;

	aspect_ratio = (float)width / (float)height;
	inverse_aspect_ratio = (float)height / (float)width;

	if (window_resize_callback) {
		window_resize_callback(width, height);
	}
}