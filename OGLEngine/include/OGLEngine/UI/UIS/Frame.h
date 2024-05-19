#pragma once

#include "../UI.h"

namespace T1WD {
	extern "C" OGLENGINE_API class Frame : public UI {
	public:
		OGLENGINE_API Frame();
		OGLENGINE_API void Render(float pos_scale_x, float pos_scale_y, float scale_x, float scale_y);

		Color color;
	};
}