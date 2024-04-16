#include "LibraryManager.h"

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool LibraryManager::is_glfw_initialized = false;
bool LibraryManager::is_glew_initialized = false;

void LibraryManager::InitializeGLFW() {
	if (is_glfw_initialized)return;

	if (!glfwInit()) {
		printf("Failed to initialize glfw\n");
		return;
	}

	is_glfw_initialized = true;
}
void LibraryManager::InitializeGLEW() {
	if (is_glew_initialized)return;

	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize glew\n");
		glfwTerminate();
		is_glfw_initialized = false;
		is_glew_initialized = false;
		return;
	}
	is_glew_initialized = true;
}
void LibraryManager::TerminateGLFW() {
	glfwTerminate();
	is_glfw_initialized = false;
	is_glew_initialized = false;
}