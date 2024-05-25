#include "Transform.h"

#include "GameObject.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/glm.hpp"

using namespace T1WD;
Transform::Transform() {
	global_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	global_rotation = rotation;
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


void Transform::OnPositionUpdate() {

	CalculateGlobalPosition();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}

void Transform::SetRotation(glm::quat rotation) {
	this->rotation = rotation;
	CalculateGlobalRotation();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void Transform::SetGlobalRotation(glm::quat rotation) {
	glm::quat parent_global_rotation(1.0f, 0.0f, 0.0f, 0.0f);
	if (gameobject->parent)
		parent_global_rotation = gameobject->parent->GetGlobalRotation();

	this->rotation = rotation - parent_global_rotation;
	CalculateGlobalRotation();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void Transform::Rotate(glm::quat rotation) {
	this->rotation = glm::normalize(rotation * this->rotation);
	CalculateGlobalRotation();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void Transform::Rotate(float angle, glm::vec3& axis) {
	float sin_half_angle = sin(glm::radians(angle));
	float cos_half_angle = cos(glm::radians(angle));

	float rx = axis.x * sin_half_angle;
	float ry = axis.y * sin_half_angle;
	float rz = axis.z * sin_half_angle;

	glm::quat rot(cos_half_angle, rx, ry, rz);
	rot = glm::conjugate(rot);
	this->rotation *= rot;
	CalculateGlobalRotation();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void Transform::OnRotationUpdate() {

	CalculateGlobalRotation();
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
	this->scale *= scale;
	CalculateGlobalScale();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}

void Transform::OnScaleUpdate() {
	CalculateGlobalScale();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}

void Transform::CalculateGlobalPosition() {
	this->global_position = position;
	if (gameobject->parent != nullptr)
		global_position += gameobject->parent->GetGlobalPosition();
}
void Transform::CalculateGlobalRotation() {
	this->global_rotation = rotation;
	//if (gameobject->parent != nullptr)
		//global_rotation *= gameobject->parent->GetGlobalRotation();
}

void Transform::CalculateGlobalScale() {
	this->global_scale = scale;
	if (gameobject->parent != nullptr)
		global_scale += gameobject->parent->GetGlobalScale();
}

void Transform::UpdateChildrenTransformations() {
	for (GameObject* object : gameobject->children) {
		Transform transform = object->transform;
		transform.CalculateGlobalPosition();
		transform.CalculateGlobalRotation();
		transform.CalculateGlobalScale();
		transform.GenerateTransformationMatrix();
	}
}
void Transform::GenerateTransformationMatrix() {
	transformation_matrix = glm::mat4x4(1.0f);
	transformation_matrix = glm::translate(transformation_matrix, global_position);
	transformation_matrix *= glm::toMat4(global_rotation);
	transformation_matrix = glm::scale(transformation_matrix, global_scale);
}