#include "Frame.h"

#include <iostream>

#include "OGLEngine/Object/GameObject.h"
#include "OGLEngine/Display/Shader/Shaders.h"

using namespace T1WD;

Frame::Frame() {
	name = "Frame";
}
Frame::~Frame() {

}

void Frame::Awake() {
	
}

void Frame::Draw() {
	Shaders::ui_frame_shader.Start();
	glm::vec3& pos = gameobject->GetGlobalPosition();
	Shaders::ui_frame_shader.LoadVec2(Shaders::ui_frame_shader.Get(1), pos.x, pos.y);
	Shaders::ui_frame_shader.LoadVec2(Shaders::ui_frame_shader.Get(2), 0.5f, 0.5f);
	UI::square_mesh.Draw();
}