#include "Shaders.h"

using namespace T1WD;


Shader Shaders::basic_mesh_3d_shader;
Shader Shaders::ui_frame_shader;

void Shaders::Init() {
	//Basic Mesh 3D Shader
	const char* vertex_data = "#version 330 core \nuniform mat4 projection_matrix; uniform mat4 view_matrix; uniform mat4 transformation_matrix; in vec3 position; void main(){gl_Position = projection_matrix * view_matrix * transformation_matrix * vec4(position, 1.0);}";
	const char* fragment_data = "#version 330 core \n out vec4 frag_color; void main(){frag_color = vec4(0.8, 0.8, 0.8, 1.0);}";
	basic_mesh_3d_shader.CreateShader(vertex_data, fragment_data);
	basic_mesh_3d_shader.AddUniform("projection_matrix");
	basic_mesh_3d_shader.AddUniform("view_matrix");
	basic_mesh_3d_shader.AddUniform("transformation_matrix");

	//UI Frame Shader
	vertex_data = "#version 330 core \n uniform vec2 position; uniform vec2 scale; in vec2 vertex; void main(){gl_Position = vec4((vertex * scale) + position, 0.0, 1.0);}";
	fragment_data = "#version 330 core \n out vec4 frag_color; void main(){frag_color = vec4(0.25, 0.25, 0.25, 1.0);}";
	ui_frame_shader.CreateShader(vertex_data, fragment_data);
	ui_frame_shader.BindAttribute(0, "vertex");
	ui_frame_shader.AddUniform("orthographic");
	ui_frame_shader.AddUniform("position");
	ui_frame_shader.AddUniform("scale");
}
void Shaders::CleanUp() {
	basic_mesh_3d_shader.CleanUp();
	ui_frame_shader.CleanUp();
}