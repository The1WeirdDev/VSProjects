#pragma once

#include <functional>

#include <OGLENGINE/OGLEngine.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace T1WD {
	extern "C" OGLENGINE_API class Display {
	public:
		static OGLENGINE_API void Create(int width, int height, const char* title);
		static OGLENGINE_API void AddWindowCallbacks();
		static OGLENGINE_API void Destroy();

		static OGLENGINE_API void CenterWindow();

		static OGLENGINE_API void SetTitle(const char* title);

		static OGLENGINE_API void ClearColors();	//Clears color buffer
		static OGLENGINE_API void ClearDepth();//Clears depth buffer. Things drawn after this will overwrite existing pixels on screen

		static OGLENGINE_API bool ShouldUpdateWindow();

		static OGLENGINE_API void PollEvents();//Updates events like key events, mouse events, window events, etc
		static OGLENGINE_API void SwapBuffers();//Actually updates the screen buffer and swaps it out with what we have drawn

		static OGLENGINE_API void OnWindowResize(GLFWwindow* window, int width, int height);
	public:
		static GLFWwindow* window;
		static int width, height;
		static float aspect_ratio, inverse_aspect_ratio;

	public:
		//Callback
		static std::function<void(int, int)> window_resize_callback;
	};
}