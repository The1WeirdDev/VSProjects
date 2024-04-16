#include "Frame.h"

#include <OGLEngine/Display/Rendering/UIRenderer.h>

Frame::Frame() : UI() {

}
void Frame::Render(float pos_scale_x, float pos_scale_y, float scale_x, float scale_y) {
	UIRenderer::RenderFrame(*this, pos_scale_x, pos_scale_y, scale_x, scale_y);
}