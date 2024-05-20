#include "Transform.h"

#include "OGLEngine/Object/GameObject.h"

#include <glm/gtc/matrix_transform.hpp>

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
	glm::vec3 parents_global_pos(0.0);
	if (gameobject->parent)
		parents_global_pos = gameobject->parent->GetGlobalPosition();

	this->position = position - parents_global_pos;
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
	if (gameobject->parent != nullptr)
		global_position += gameobject->parent->GetGlobalPosition();
	return global_position;
}

void Transform::UpdateChildrenTransformations() {
	for (GameObject* object : gameobject->children) {
		Transform transform = object->transform;
		transform.CalculateGlobalPosition();
		transform.GenerateTransformationMatrix();
	}
}
void Transform::GenerateTransformationMatrix() {
	transformation_matrix = glm::mat4x4(1.0f);
	transformation_matrix = glm::translate(transformation_matrix, this->global_position);
}