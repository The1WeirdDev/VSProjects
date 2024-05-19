#pragma once

#include "OGLEngine.h"

namespace T1WD {
	class LibraryManager {
	public:
		static OGLENGINE_API void InitializeGLFW();
		static OGLENGINE_API void InitializeGLEW();

		static OGLENGINE_API void TerminateGLFW();

		static bool is_glfw_initialized;
		static bool is_glew_initialized;
	};
}