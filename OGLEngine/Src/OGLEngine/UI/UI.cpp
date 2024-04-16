#include "UI.h"

#include <iostream>

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