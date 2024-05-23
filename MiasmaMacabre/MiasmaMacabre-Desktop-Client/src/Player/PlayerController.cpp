#include "PlayerController.h"

#include <OGLEngine/Input/Input.h>
#include <OGLEngine/Time/Time.h>
#include <iostream>

#include <GL/Glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/euler_angles.hpp>

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
float getYawFromQuaternion(const glm::quat& q) {
	// Extract the yaw angle from the quaternion
	return atan2(2.0f * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
}
void PlayerController::Update() {
	float speed = Time::delta_time * 10.0f;

	glm::quat q = gameobject->GetRotation();
	float new_Yaw = glm::eulerAngles(q).y;

	//Not giving correct values
	std::cout << new_Yaw << " " << suppose_yaw << std::endl;
	float yaw = glm::yaw(q);
	glm::vec3 dir(0.0f);
	//std::cout << (yaw * (180.0f / 3.14159f)) << std::endl;

	if (Input::IsKeyDown(GLFW_KEY_W)) {
		dir.x -= sin(yaw);
		dir.z += cos(yaw);
	}else if (Input::IsKeyDown(GLFW_KEY_S)) {
		dir.x += sin(yaw);
		dir.z -= cos(yaw);
	}
	gameobject->transform.Translate(dir * speed);
	

	float turn_x = Time::delta_time * Input::delta_x;
	float turn_y = Time::delta_time * Input::delta_y;
	suppose_yaw += turn_x;
	gameobject->transform.Rotate(glm::quat(glm::vec3(0.0f, turn_x, 0.0f)));
}
void PlayerController::Draw() {
}