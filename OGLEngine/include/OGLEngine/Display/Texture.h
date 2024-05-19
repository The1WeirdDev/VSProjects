#pragma once

#include <OGLEngine/OGLEngine.h>

namespace T1WD {
	extern "C" OGLENGINE_API class Texture {
	public:
		static int CreateTexture(const char* location);
	};
}