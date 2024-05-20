#include "UIRenderer.h"
#include <iostream>

#include <OGLEngine/Display/Display.h>
#include <OGLEngine/Display/Font/Font.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <cstdio>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb/stb_truetype.h"

using namespace T1WD;
Shader UIRenderer::frame_shader;
UITexturedMesh UIRenderer::texture_mesh;
Shader UIRenderer::text_label_shader;

//TODO: Handle coordinate system

void UIRenderer::Init() {
	/* load font file */
	long size;
	unsigned char* fontBuffer;

	std::ifstream fontFile("res/ariel.ttf", std::ios::binary | std::ios::ate);

	if (!fontFile) {
		std::cout << "Couldn't find file" << std::endl;
		return;
	}
	std::cout << "Found file" << std::endl;
	size = fontFile.tellg(); /* how long is the file ? */
	fontFile.seekg(0, std::ios::beg);

	printf("FILE SIZE %d\n", size);
	fontBuffer = new unsigned char[size];
	fontFile.read((char*)fontBuffer, size);
	//fread(fontBuffer, size, 1, fontFile);
	fontFile.close();

	/* prepare font */
	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, fontBuffer, 0))
	{
		printf("failed\n");
	}
	printf("Loaded\n");

	int b_w = 512; /* bitmap width */
	int b_h = 128; /* bitmap height */
	int l_h = 64; /* line height */

	/* create a bitmap for the phrase */
	unsigned char* bitmap = new unsigned char[b_w * b_h];

	/* calculate font scaling */
	float scale = stbtt_ScaleForPixelHeight(&info, l_h);

	const char* word = "the quick brown fox";

	int x = 0;
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

	ascent = roundf(ascent * scale);
	descent = roundf(descent * scale);

	int i;
	for (i = 0; i < strlen(word); ++i)
	{
		/* how wide is this character */
		int ax;
		int lsb;
		stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);
		/* (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].) */

		/* get bounding box for character (may be offset to account for chars that dip above or below the line) */
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		/* compute y (different characters have different heights) */
		int y = ascent + c_y1;

		/* render character (stride and offset is important here) */
		int byteOffset = x + roundf(lsb * scale) + (y * b_w);
		stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

		/* advance x */
		x += roundf(ax * scale);

		/* add kerning */
		int kern;
		kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
		x += roundf(kern * scale);
	}

	stbi_write_png("out.png", b_w, b_h, 1, bitmap, b_w);


	unsigned char* vertices = new unsigned char[8] {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};
	float* texture_coords = new float[8] {
		1,1,
		1,0,
		0,1,
		0,0
	};
	unsigned char* indices = new unsigned char[6] {
		0, 1, 2, 2, 1, 3
	};
	texture_mesh.Create(vertices, 8, texture_coords, 8, indices, 6);

	printf("Creating UI Shaders.\n");
	//Frame Shader
	const char* vertex_data = "#version 330 core \nin vec2 position; uniform vec2 ui_pos; uniform vec2 size; void main(){gl_Position = vec4(ui_pos + (position * size), 0.0, 1.0);}";
	const char* fragment_data = "#version 330 core \n uniform vec3 color; out vec4 frag_color; void main(){frag_color = vec4(color, 1.0);}";
	frame_shader.CreateShader(vertex_data, fragment_data);

	frame_shader.BindAttribute(0, "position");

	frame_shader.uniforms.resize(3);
	frame_shader.uniforms[0] = frame_shader.GetUniformLocation("ui_pos");
	frame_shader.uniforms[1] = frame_shader.GetUniformLocation("size");
	frame_shader.uniforms[2] = frame_shader.GetUniformLocation("color");

	//Text Label
	vertex_data = "#version 330 core \nin vec2 position; in vec2 in_text_coords; out vec2 text_coords; uniform vec2 ui_pos; uniform vec2 size; void main(){text_coords = in_text_coords; gl_Position = vec4(ui_pos + (position * size), 0.0, 1.0);}";
	fragment_data = "#version 330 core \n uniform vec3 color; in vec2 text_coords; out vec4 frag_color; uniform sampler2D tex; void main(){frag_color = texture2D(tex, text_coords).a * vec4(1,1,0,1);}";
	text_label_shader.CreateShader(vertex_data, fragment_data);

	text_label_shader.BindAttribute(0, "position");
	text_label_shader.BindAttribute(1, "in_text_coords");

	text_label_shader.uniforms.resize(3);
	text_label_shader.uniforms[0] = text_label_shader.GetUniformLocation("ui_pos");
	text_label_shader.uniforms[1] = text_label_shader.GetUniformLocation("size");
	text_label_shader.uniforms[2] = text_label_shader.GetUniformLocation("color");
	glActiveTexture(GL_TEXTURE0);

	printf("Initialized UI Renderer.\n");
}
void UIRenderer::RenderFrame(Frame& frame, float global_pos_scale_x, float global_pos_scale_y, float global_scale_x, float global_scale_y) {
	if (frame.is_renderable == false)return;

	frame_shader.Start();

	//Size
	switch (frame.screen_size_restraint) {
	case ScreenSizeRestraint::AspectX:
		global_scale_x *= Display::aspect_ratio;
		break;
	case ScreenSizeRestraint::AspectY:
		global_scale_y *= Display::aspect_ratio;
		break;
	case ScreenSizeRestraint::InverseAspectX:
		global_scale_x *= Display::inverse_aspect_ratio;
		break;
	case ScreenSizeRestraint::InverseAspectY:
		global_scale_y *= Display::inverse_aspect_ratio;
		break;
	}

	global_pos_scale_x += frame.x * global_scale_x;
	global_pos_scale_y += frame.y * global_scale_y;

	global_scale_x *= frame.width;
	global_scale_y *= frame.height;

	frame_shader.LoadVec2(frame_shader.uniforms[0], global_pos_scale_x, global_pos_scale_y);
	frame_shader.LoadVec2(frame_shader.uniforms[1], global_scale_x, global_scale_y);
	frame_shader.LoadVec3(frame_shader.uniforms[2], frame.color.r, frame.color.g, frame.color.b);

	texture_mesh.Draw();

	for (int i = 0; i < frame.children.size(); i++) {
		UI* child = frame.children[i];
		
		switch (child->position_restraint) {
		case UIRestraint::Global:
			global_pos_scale_x = 0;
			global_pos_scale_y = 0;
			break;
		}
		switch (child->size_restraint) {
		case UIRestraint::Global:
			global_scale_x = 1.0f;
			global_scale_y = 1.0f;
			break;
		}
		child->Render(global_pos_scale_x, global_pos_scale_y, global_scale_x, global_scale_y);
	}
}
void UIRenderer::RenderTextLabel(TextLabel& text_label, float global_pos_scale_x, float global_pos_scale_y, float global_scale_x, float global_scale_y) {
	if (text_label.is_renderable == false)return;

	text_label_shader.Start();

	//Size
	switch (text_label.screen_size_restraint) {
	case ScreenSizeRestraint::AspectX:
		global_scale_x *= Display::aspect_ratio;
		break;
	case ScreenSizeRestraint::AspectY:
		global_scale_y *= Display::aspect_ratio;
		break;
	case ScreenSizeRestraint::InverseAspectX:
		global_scale_x *= Display::inverse_aspect_ratio;
		break;
	case ScreenSizeRestraint::InverseAspectY:
		global_scale_y *= Display::inverse_aspect_ratio;
		break;
	}

	global_pos_scale_x += text_label.x * global_scale_x;
	global_pos_scale_y += text_label.y * global_scale_y;

	global_scale_x *= text_label.width;
	global_scale_y *= text_label.height;

	text_label_shader.LoadVec2(text_label_shader.uniforms[0], global_pos_scale_x, global_pos_scale_y);
	text_label_shader.LoadVec2(text_label_shader.uniforms[1], global_scale_x, global_scale_y);
	text_label_shader.LoadVec3(text_label_shader.uniforms[2], text_label.color.r, text_label.color.g, text_label.color.b);

	//glBindTexture(GL_TEXTURE_2D, Font::texture);
	texture_mesh.Draw();

	for (int i = 0; i < text_label.children.size(); i++) {
		UI* child = text_label.children[i];

		switch (child->position_restraint) {
		case UIRestraint::Global:
			global_pos_scale_x = 0;
			global_pos_scale_y = 0;
			break;
		}
		switch (child->size_restraint) {
		case UIRestraint::Global:
			global_scale_x = 1.0f;
			global_scale_y = 1.0f;
			break;
		}
		child->Render(global_pos_scale_x, global_pos_scale_y, global_scale_x, global_scale_y);
	}
}