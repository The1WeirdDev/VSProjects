#include "UI.h"

#include <iostream>

#include <OGLEngine/Display/Display.h>

namespace T1WD {
	UI::UI() {

	}
	UI::~UI() {

	}

	void UI::Render(float pos_scale_x, float pos_scale_y, float scale_x, float scale_y) {
		printf("UI Render\n");
	}
	void UI::SetPosition(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void UI::SetSize(float x, float y) {
		width = x;
		height = y;
	}

	void UI::AppendChild(UI& ui) {
		ui.parent = this;
		children.push_back(&ui);
	}

	float UI::GetScreenRestrainedWidth() {
		switch (screen_size_restraint) {
		case ScreenSizeRestraint::AspectX:
			return width * Display::aspect_ratio;
		case ScreenSizeRestraint::InverseAspectX:
			return width * Display::inverse_aspect_ratio;
		default:
			return width;
		}
	}
}