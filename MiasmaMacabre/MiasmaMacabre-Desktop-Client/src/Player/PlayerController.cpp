#include "PlayerController.h"

#include <OGLEngine/Input/Input.h>
#include <OGLEngine/Time/Time.h>
#include <iostream>

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
}
void PlayerController::Draw() {
}