#include "Basic3DMesh.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace T1WD;
Basic3DMesh::Basic3DMesh() {

}
Basic3DMesh::~Basic3DMesh() {}


void Basic3DMesh::Create(float* vertex_data, size_t vertex_count, unsigned int* index_data, size_t index_count) {
	glCreateVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	//Vertices
	glCreateBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count, vertex_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

	glEnableVertexAttribArray(0);

	//Indices
	glCreateBuffers(1, &ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count, index_data, GL_STATIC_DRAW);

	this->index_count = index_count;
	is_created = true;
}
void Basic3DMesh::Draw() {
	if (is_created == false)return;
	glBindVertexArray(vao_id);

	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
}
void Basic3DMesh::CleanUp() {
	glBindVertexArray(0);
	
	glDeleteBuffers(1, &ebo_id);
	glDeleteBuffers(1, &vbo_id);
	glDeleteVertexArrays(1, &vao_id);

	vbo_id = 0;
	vao_id = 0;
	index_count = 0;
	is_created = false;
}