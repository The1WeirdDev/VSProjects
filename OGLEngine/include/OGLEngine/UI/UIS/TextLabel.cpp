#include "TextLabel.h"

#include <OGLEngine/Display/Rendering/UIRenderer.h>

#include "ttf2mesh.h"

using namespace T1WD;

TextLabel::TextLabel() : UI() {

}
void TextLabel::Render(float pos_scale_x, float pos_scale_y, float scale_x, float scale_y) {
	UIRenderer::RenderTextLabel(*this, pos_scale_x, pos_scale_y, scale_x, scale_y);
}
void TextLabel::SetText(const char* text) {
	this->text = text;
}
void TextLabel::GenerateMesh() {
	
}