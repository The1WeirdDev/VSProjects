#include "BasicMeshImpersonator.h"

#include "OGLEngine/Display/Shader/Shaders.h"
#include "OGLEngine/Object/GameObject.h"

using namespace T1WD;
BasicMeshImpersonator::BasicMeshImpersonator(){}
BasicMeshImpersonator::~BasicMeshImpersonator(){}

void BasicMeshImpersonator::SetMesh(BasicMesh* mesh) {
	this->mesh = mesh;
}

void BasicMeshImpersonator::Draw() {
	if (mesh == nullptr)return;

	Shaders::basic_mesh_3d_shader.Start();
	glm::mat4& matrix = gameobject->GetTransformationMatrix();
	Shaders::basic_mesh_3d_shader.LoadMat4x4(Shaders::basic_mesh_3d_shader.uniforms[2], matrix);
	mesh->Draw();
}