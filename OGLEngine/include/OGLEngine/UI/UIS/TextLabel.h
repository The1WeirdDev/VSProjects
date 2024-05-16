#pragma once

#include <string>
#include "../UI.h"

extern "C" OGLENGINE_API class TextLabel : public UI {
public:
	OGLENGINE_API TextLabel();
	OGLENGINE_API void Render(float pos_scale_x, float pos_scale_y, float scale_x, float scale_y);

	OGLENGINE_API void SetText(const char* text);
	OGLENGINE_API void GenerateMesh();
	Color color;
	std::string text;
};

