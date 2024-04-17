#pragma once

#include <vector>
#include <OGLEngine/OGLEngine.h>

enum ScreenSizeRestraint {
	NONE = 0,
	AspectX, //X is multiplied by aspect
	AspectY, //Y is multiplied by aspect
	InverseAspectX, //X is multiplied by inverse aspect
	InverseAspectY, //Y is multiplied by inverse aspect
};

enum UIRestraint {
	Relative = 0,
	Global
};

extern "C" OGLENGINE_API struct Color {
public:
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;

	Color() {}

	Color(float r, float g, float b) {
		this->r = r;
		this->b = b;
		this->g = g;
	}
};

extern "C" OGLENGINE_API class UI {
public:
	UI();
	~UI();

	virtual OGLENGINE_API void Render(float pos_scale_x, float pos_scale_y, float scale_x, float scale_y);
	OGLENGINE_API void SetPosition(float x, float y);
	OGLENGINE_API void SetSize(float x, float y);

	OGLENGINE_API void AppendChild(UI& ui);
	OGLENGINE_API float GetScreenRestrainedWidth();
public:

	UI* parent = nullptr;
	std::vector<UI*> children;

	bool is_renderable = true;//If is renderable Will attempt to draw itself and children
	bool is_visible = true;//If enabled will render the current frame

	float x = 0;
	float y = 0;
	float width = 1.0f;
	float height = 1.0f;
	
	ScreenSizeRestraint screen_size_restraint = ScreenSizeRestraint::NONE;
	UIRestraint size_restraint = UIRestraint::Relative;
	UIRestraint position_restraint = UIRestraint::Relative;
};