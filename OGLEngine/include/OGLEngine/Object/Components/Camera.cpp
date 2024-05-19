#include "Camera.h"

#include "../GameObject.h"
#include "../../Scene/Scene.h"
#include <iostream>

using namespace T1WD;
Camera::Camera() : Component(){

}
Camera::~Camera() {

}

void Camera::Awake() {
	if (gameobject->scene->camera == nullptr) {
		gameobject->scene->camera = this;
		return;
	}

	printf("Multiple camera in one scene\n");
}

void Camera::Update() {

}