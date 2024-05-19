#pragma once

#include <OGLEngine/OGLEngine.h>

namespace T1WD {
	extern "C" OGLENGINE_API class Time {
	public:
		static OGLENGINE_API void Init();
		static OGLENGINE_API void Update();

		static double time;
		static double last_time;
		static double delta_time;
	};
}