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
	float speed = Time::delta_time;
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
	gameobject->transform.Translate(direction * speed);
}
void PlayerController::Draw() {
}