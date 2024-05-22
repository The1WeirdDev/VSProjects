#include "PlayerController.h"

#include <OGLEngine/Input/Input.h>
#include <OGLEngine/Time/Time.h>
#include <iostream>

#include <GL/Glew.h>
#include <GLFW/glfw3.h>

using namespace T1WD;
PlayerController::PlayerController() {
	name = "PlayerController";
}
PlayerController::~PlayerController() {

}
void PlayerController::Awake() {
	camera = (T1WD::Camera*)gameobject->GetComponent("Camera"); 
}
void PlayerController::CleanUp() {
}

void PlayerController::Update() {
	float speed = Time::delta_time * 10.0f;
	glm::vec3 direction(0);

	if (Input::IsKeyDown(GLFW_KEY_A)) {
		direction.x -= 1;
	}
	if (Input::IsKeyDown(GLFW_KEY_D)) {
		direction.x += 1;
	}
	if (Input::IsKeyDown(GLFW_KEY_W)) {
		direction.z -= 1;
	}
	if (Input::IsKeyDown(GLFW_KEY_S)) {
		direction.z += 1;
	}
	if (Input::IsKeyDown(GLFW_KEY_SPACE)) {
		direction.y += 1;
	}
	if (Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		direction.y -= 1;
	}

	gameobject->transform.Translate(direction * speed);
	glm::vec3 pos = gameobject->GetGlobalPosition();
}
void PlayerController::Draw() {
}