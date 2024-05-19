#pragma once

#include "OGLEngine/OGLEngine.h"
#include "OGLEngine/UI/UIS/Frame.h"
#include "OGLEngine/UI/UIS/TextLabel.h"
#include "OGLEngine/Display/Shader/Shader.h"
#include "OGLEngine/Display/Mesh/UITexturedMesh.h"
/*
MUST CALL INIT TO RENDER
*/
namespace T1WD {
	extern "C" OGLENGINE_API class UIRenderer {
	public:
		static OGLENGINE_API void Init();

		static OGLENGINE_API void RenderFrame(Frame& frame, float global_pos_scale_x = 0.0f, float global_pos_scale_y = 0.0f, float global_scale_x = 1.0f, float global_scale_y = 1.0f);
		static OGLENGINE_API void RenderTextLabel(TextLabel& text_label, float global_pos_scale_x = 0.0f, float global_pos_scale_y = 0.0f, float global_scale_x = 1.0f, float global_scale_y = 1.0f);
	private:
		static Shader frame_shader;
		static Shader text_label_shader;
		static UITexturedMesh texture_mesh;
	};
}