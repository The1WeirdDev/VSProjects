#include "Transform.h"

#include "OGLEngine/Object/GameObject.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace T1WD;
Transform::Transform() {
	global_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
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



void Transform::SetScale(glm::vec3 scale) {
	this->scale = scale;
	CalculateGlobalScale();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void Transform::SetGlobalScale(glm::vec3 scale) {
	glm::vec3 parents_global_scale(0.0);
	if (gameobject->parent)
		parents_global_scale = gameobject->parent->GetGlobalScale();

	this->scale = scale - parents_global_scale;
	CalculateGlobalScale();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void Transform::Scale(glm::vec3 scale) {
	std::cout << this->scale.x << " " << this->scale.y << " " << this->scale.z << std::endl;
	this->scale *= scale;
	std::cout << scale.x << " " << scale.y << " " << scale.z << std::endl;
	std::cout << this->scale.x << " " << this->scale.y << " " << this->scale.z << std::endl;
	CalculateGlobalScale();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}

void Transform::OnScaleUpdate() {
	CalculateGlobalScale();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}

glm::vec3 Transform::CalculateGlobalPosition() {
	this->global_position = position;
	if (gameobject->parent != nullptr)
		global_position += gameobject->parent->GetGlobalPosition();
	return global_position;
}

glm::vec3 Transform::CalculateGlobalScale() {
	this->global_scale = scale;
	if (gameobject->parent != nullptr)
		global_scale += gameobject->parent->GetGlobalScale();
	return global_scale;
}

void Transform::UpdateChildrenTransformations() {
	for (GameObject* object : gameobject->children) {
		Transform transform = object->transform;
		transform.CalculateGlobalPosition();
		transform.CalculateGlobalScale();
		transform.GenerateTransformationMatrix();
	}
}
void Transform::GenerateTransformationMatrix() {
	transformation_matrix = glm::mat4x4(1.0f);
	transformation_matrix = glm::translate(transformation_matrix, global_position);
	transformation_matrix = glm::scale(transformation_matrix, global_scale);
}