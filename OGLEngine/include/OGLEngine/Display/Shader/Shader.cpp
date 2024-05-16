#include "Shader.h"

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Shader::Shader() {
	
}

Shader::~Shader() {
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	glDeleteProgram(program_id);
	vertex_shader_id = 0;
	fragment_shader_id = 0;
	program_id = 0;
}
void Shader::Start() {
	glUseProgram(program_id);
}
void Shader::Stop() {
	glUseProgram(0);
}
void Shader::BindAttribute(int position, const char* name) {
	glBindAttribLocation(program_id, position, name);
}
int Shader::GetUniformLocation(const char* name) {
	return glGetUniformLocation(program_id, name);
}

void Shader::CreateShader(const char* vertex_shader_source, const char* fragment_shader_source) {
	vertex_shader_id = CreateShader(vertex_shader_source, GL_VERTEX_SHADER);
	fragment_shader_id = CreateShader(fragment_shader_source, GL_FRAGMENT_SHADER);
	program_id = glCreateProgram();

	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);

	glLinkProgram(program_id);
	glValidateProgram(program_id);

	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);
}
int Shader::CreateShader(const char* shader_data, int shader_type) {
	int shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_data, nullptr);
	glCompileShader(shader);

	int status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		const char* type = (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::string message = "Failed to create ";
		printf((message + type + " shader.\n\n").c_str());

		int max_length = 520;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(max_length);
		glGetShaderInfoLog(shader, max_length, &max_length, &errorLog[0]);
		std::cout << errorLog.data() << std::endl;
		glDeleteShader(shader);

	}
	return shader;
}
void Shader::LoadVec2(int location, float x, float y){
	glUniform2f(location, x, y);
}
void Shader::LoadVec3(int location, float x, float y, float z) {
	glUniform3f(location, x, y, z);
}