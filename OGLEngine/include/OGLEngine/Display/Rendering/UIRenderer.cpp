#include "UIRenderer.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <OGLEngine/Display/Display.h>
#include <OGLEngine/Display/Font/Font.h>


#include <ft2build.h>
#include FT_FREETYPE_H  

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb/stb_truetype.h"
/*
using namespace T1WD;
Shader UIRenderer::frame_shader;
Shader UIRenderer::text_label_shader;
Shader UIRenderer::text_shader;
UITexturedMesh UIRenderer::texture_mesh;

#include <ft2build.h>
#include FT_FREETYPE_H

//TODO: Handle coordinate system

static FT_Library ft;
static FT_Face face;
struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

static std::map<char, Character> Characters;
static unsigned int VAO, VBO;

void UIRenderer::Init() {
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	if (FT_New_Face(ft, "res/ariel.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		return;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
	//Text Shader
	vertex_data = "#version 330 core \nin vec4 vertex; out vec2 text_coords; uniform mat4 projection; void main(){text_coords = vertex.zw; gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);}";
	fragment_data = "#version 330 core \n uniform vec3 color; in vec2 text_coords; out vec4 frag_color; uniform sampler2D tex; void main(){frag_color = texture2D(tex, text_coords).a * vec4(1,1,0,1);}";
	text_shader.CreateShader(vertex_data, fragment_data);

	text_shader.BindAttribute(0, "position");
	text_shader.BindAttribute(1, "in_text_coords");

	text_shader.uniforms.resize(1);
	text_shader.uniforms[0] = text_label_shader.GetUniformLocation("projection");
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
	text_shader.Start();
	glm::mat4 mat = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	text_shader.LoadMat4x4(text_shader.uniforms[0], mat);
	//Render Text
	glBindVertexArray(VAO);
	printf("Rendering Text\n");
	std::string text = "HELLO";
	int x = 0, y = 0;
	float scale = 1.0f;
	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}*/