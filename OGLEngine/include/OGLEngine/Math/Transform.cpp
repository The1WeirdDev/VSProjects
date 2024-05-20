#include "Transform.h"

#include "OGLEngine/Object/GameObject.h"

using namespace T1WD;
Transform::Transform() {

}
Transform::~Transform() {

}
void Transform::SetPosition(glm::vec3 position) {
	//Relative Position
	this->position = position;
	CalculateGlobalPosition();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void Transform::SetGlobalPosition(glm::vec3 position) {
	this->position = position - GetParentsGlobalPosition();
	CalculateGlobalPosition();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();

}
void Transform::Translate(glm::vec3 position) {
	this->position += position;
	CalculateGlobalPosition();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}

glm::vec3 Transform::CalculateGlobalPosition() {
	this->global_position = position;
	if (this->parent != nullptr)
		global_position += this->parent->global_position;
	return global_position;
}

glm::vec3 Transform::GetParentsGlobalPosition() {
	if (gameobject->parent)
		return gameobject->parent->global_position;
	return glm::vec3(0.0);
}
void Transform::UpdateChildrenTransformations() {
	for (GameObject* object : children) {
		object->CalculateGlobalPosition();
		object->GenerateTransformationMatrix();
	}
}
void Transform::GenerateTransformationMatrix() {
	transformation_matrix = glm::mat4x4(1.0f);
	transformation_matrix = glm::translate(transformation_matrix, this->global_position);
}