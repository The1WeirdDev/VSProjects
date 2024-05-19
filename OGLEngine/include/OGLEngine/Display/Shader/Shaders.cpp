#include "Shaders.h"

using namespace T1WD;


Shader Shaders::basic_mesh_3d_shader;

void Shaders::Init() {
	const char* vertex_data = "#version 330 core \nuniform mat4 projection_matrix; uniform mat4 transformation_matrix; in vec3 position; void main(){gl_Position = projection_matrix * transformation_matrix * vec4(position, 1.0);}";
	const char* fragment_data = "#version 330 core \n out vec4 frag_color; void main(){frag_color = vec4(0.8, 0.8, 0.8, 1.0);}";
	basic_mesh_3d_shader.CreateShader(vertex_data, fragment_data);
	basic_mesh_3d_shader.uniforms.push_back(basic_mesh_3d_shader.GetUniformLocation("projection_matrix"));
	basic_mesh_3d_shader.uniforms.push_back(basic_mesh_3d_shader.GetUniformLocation("transformation_matrix"));
}
void Shaders::CleanUp() {
	basic_mesh_3d_shader.CleanUp();
}