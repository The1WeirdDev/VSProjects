#include "Display.h"

#include <stdio.h>

#include <OGLEngine/LibraryManager.h>

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
	glfwSetWindowSizeCallback(window, Display::OnWindowResize);

	Display::width = width;
	Display::height = height;
	aspect_ratio = (float)width / (float)height;
	inverse_aspect_ratio = (float)height / (float)width;

	glViewport(0, 0, width, height);
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