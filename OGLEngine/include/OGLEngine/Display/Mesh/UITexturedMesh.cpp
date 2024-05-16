#include "UITexturedMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


UITexturedMesh::UITexturedMesh() {

}

void UITexturedMesh::Create(unsigned char* vertices, unsigned char vertex_count, float* texture_coords, unsigned char texture_coord_count, unsigned char* indices, unsigned char index_count) {
	glCreateVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	//Vertices
	glCreateBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * vertex_count, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_UNSIGNED_BYTE, false, 0, nullptr);

	//Texture Coords
	glCreateBuffers(1, &tbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, tbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texture_coord_count, texture_coords, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Indices
	glCreateBuffers(1, &ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char) * index_count, indices, GL_STATIC_DRAW);

	this->index_count = index_count;
}
void UITexturedMesh::Draw() {
	glBindVertexArray(vao_id);

	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_BYTE, nullptr);
	
	//glBindVertexArray(0);
}