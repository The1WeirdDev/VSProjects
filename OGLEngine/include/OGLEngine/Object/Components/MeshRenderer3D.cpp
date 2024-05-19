#include "MeshRenderer3D.h"

#include "OGLEngine/Display/Shader/Shaders.h"

#include "OGLEngine/Object/GameObject.h"
using namespace T1WD;
MeshRenderer3D::MeshRenderer3D() : Component(){
	name = "MeshRenderer3D";
}
MeshRenderer3D::~MeshRenderer3D() {
	mesh.CleanUp();
}

void MeshRenderer3D::Awake() {
}

void MeshRenderer3D::Draw() {
	Shaders::basic_mesh_3d_shader.Start();
	glm::mat4 matrix = gameobject->GetTransformationMatrix();
	Shaders::basic_mesh_3d_shader.LoadMat4x4(Shaders::basic_mesh_3d_shader.uniforms[1], matrix);
	mesh.Draw();
}