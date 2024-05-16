#include "Font.h"
#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
FT_Library Font::library;
FT_Face Font::face;
unsigned int Font::texture;

void Font::Init() {
	FT_Error error = FT_Init_FreeType(&library);

	if (error) {
		printf("Error initializing freetype\n");
		return;
	}

	error = FT_New_Face(library, "C:/Windows/Fonts/ariblk.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format) {
		printf("Unsupported file type for free type\n");
		return;
	}
	else if (error) {
		printf("Error with freetype face\n");
		return;
	}
	FT_GlyphSlot slot = face->glyph;
	FT_Load_Glyph(face, 66, FT_LOAD_RENDER);

	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);

}
*/