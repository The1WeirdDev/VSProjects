#include "Texture.h"

#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace T1WD;

int Texture::CreateTexture(const char* location) {
	std::ifstream file(location);
	unsigned int texture = 0;

	glCreateTextures(GL_TEXTURE_2D, 1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	file.close();
	return texture;
}