#include "GameObject.h"

#include "Component/Component.h"
#include <iostream>

using namespace T1WD;
GameObject::GameObject() {
	transform.gameobject = this;
	transform.GenerateTransformationMatrix();
}
GameObject::~GameObject() {
}

GameObject* GameObject::CreateChild() {
	GameObject* object = new GameObject();
	object->parent = this;
	object->scene = this->scene;
	children.push_back(object);
	return object;
}
Component* GameObject::AddComponent(Component* component) {
	components.push_back(component);
	component->gameobject = this;
	component->Awake();
	component->Start();
	return component;
}
Component* GameObject::GetComponent(const char* component_name) {
	for (Component* component : components) {
		if (component->name == component_name)
			return component;
	}

	return nullptr;
}
void GameObject::Awake() {
	for (Component* component : components) {
		component->Awake();
	}
	for (GameObject* child : children) {
		child->Awake();
	}
}
void GameObject::Start() {
	for (Component* component : components) {
		component->Start();
	}
	for (GameObject* child : children) {
		child->Start();
	}
}
void GameObject::CleanUp() {
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->CleanUp();
		delete components[i];
	}

	for (size_t i = 0; i < children.size(); i++) {
		children[i]->CleanUp();
		delete children[i];
	}
	//No need to resize vector because gameobject is getting resized
}

void GameObject::Update() {
	for (Component* component : components) {
		component->Update();
	}
	for (GameObject* child : children) {
		child->Update();
	}
}
void GameObject::Draw() {
	for (Component* component : components) {
		component->Draw();
	}
	for (GameObject* child : children) {
		child->Draw();
	}
}
void GameObject::LateDraw() {
	for (Component* component : components) {
		component->LateDraw();
	}
	for (GameObject* child : children) {
		child->LateDraw();
	}
}

void GameObject::SetPosition(glm::vec3 position) {
	//Relative Position
	transform.SetPosition(position);
}
void GameObject::SetGlobalPosition(glm::vec3 position) {
	transform.SetGlobalPosition(position);

}
void GameObject::Translate(glm::vec3 position) {
	transform.Translate(position);
}


void GameObject::SetScale(glm::vec3 scale) {
	transform.SetScale(scale);
}
void GameObject::SetGlobalScale(glm::vec3 scale) {
	transform.SetGlobalScale(scale);
}
void GameObject::Scale(glm::vec3 scale) {
	transform.Scale(scale);
}

/*

void GameObject::SetPosition(glm::vec3 position) {
	//Relative Position
	this->position = position;
	CalculateGlobalPosition();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}
void GameObject::SetGlobalPosition(glm::vec3 position) {
	this->position = position - GetParentsGlobalPosition();
	CalculateGlobalPosition();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();

}
void GameObject::Translate(glm::vec3 position) {
	this->position += position;
	CalculateGlobalPosition();
	GenerateTransformationMatrix();
	UpdateChildrenTransformations();
}

glm::vec3 GameObject::CalculateGlobalPosition() {
	this->global_position = position;
	if (this->parent != nullptr)
		global_position += this->parent->global_position;
	return global_position;
}

glm::vec3 GameObject::GetParentsGlobalPosition() {
	if (this->parent)
		return this->parent->global_position;
	return glm::vec3(0.0);
}
void GameObject::UpdateChildrenTransformations() {
	for (GameObject* object : children) {
		object->CalculateGlobalPosition();
		object->GenerateTransformationMatrix();
	}
}
void GameObject::GenerateTransformationMatrix() {
	transformation_matrix = glm::mat4x4(1.0f);
	transformation_matrix = glm::translate(transformation_matrix, this->global_position);
}
*/