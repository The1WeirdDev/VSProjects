#pragma once

#include <functional>

#include <OGLENGINE/OGLEngine.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern "C" OGLENGINE_API class Display {
public:
	static OGLENGINE_API void Create(int width, int height, const char* title);
	static OGLENGINE_API void Destroy();

	static OGLENGINE_API void CenterWindow();

	static OGLENGINE_API void SetTitle(const char* title);

	static OGLENGINE_API void ClearColors();
	static OGLENGINE_API void ClearDepth();

	static OGLENGINE_API bool ShouldUpdateWindow();

	static OGLENGINE_API void PollEvents();
	static OGLENGINE_API void SwapBuffers();
	
	static OGLENGINE_API void OnWindowResize(GLFWwindow* window, int width, int height);
public:
	static GLFWwindow* window;
	static int width, height;
	static float aspect_ratio, inverse_aspect_ratio;

public:
	//Callback
	static std::function<void(int, int)> window_resize_callback;
};