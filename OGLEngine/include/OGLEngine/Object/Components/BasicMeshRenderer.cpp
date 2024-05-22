#include "BasicMeshRenderer.h"

#include "OGLEngine/Display/Shader/Shaders.h"

#include "OGLEngine/Object/GameObject.h"
using namespace T1WD;
BasicMeshRenderer::BasicMeshRenderer() : Component(){
	name = "BasicMeshRenderer";
}
BasicMeshRenderer::~BasicMeshRenderer() {
	mesh.CleanUp();
}

void BasicMeshRenderer::Awake() {
}

void BasicMeshRenderer::Draw() {
	Shaders::basic_mesh_3d_shader.Start();
	glm::mat4& matrix = gameobject->GetTransformationMatrix();
	Shaders::basic_mesh_3d_shader.LoadMat4x4(Shaders::basic_mesh_3d_shader.uniforms[2], matrix);
	mesh.Draw();
}