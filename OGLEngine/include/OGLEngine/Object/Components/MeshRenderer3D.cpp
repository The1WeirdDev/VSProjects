#include "MeshRenderer3D.h"
using namespace T1WD;
MeshRenderer3D::MeshRenderer3D() : Component(){

}
MeshRenderer3D::~MeshRenderer3D() {
	mesh.CleanUp();
}

void MeshRenderer3D::Awake() {
}

void MeshRenderer3D::Draw() {
	mesh.Draw();
}