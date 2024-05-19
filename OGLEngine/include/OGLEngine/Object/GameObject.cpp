#include "GameObject.h"

#include "Component/Component.h"
#include <iostream>

using namespace T1WD;
GameObject::GameObject() {}
GameObject::~GameObject() {
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->CleanUp();
		delete components[i];
	}

	components.resize(0);
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
	for (Component* component : components) {
		component->CleanUp();
	}
	for (GameObject* child : children) {
		child->CleanUp();
	}
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
	this->position = position;
}
void GameObject::SetGlobalPosition(glm::vec3 position) {
	glm::vec3 accumulated_position;

	GameObject* current_object = this;
	while (current_object != nullptr) {
		accumulated_position += current_object->position;
		current_object = current_object->parent;
	}
	this->position = accumulated_position - position;
	this->global_position = position;
}