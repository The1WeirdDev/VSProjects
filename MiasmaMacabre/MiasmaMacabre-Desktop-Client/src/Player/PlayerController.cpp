#include "PlayerController.h"

#include <OGLEngine/Input/Input.h>
#include <OGLEngine/Time/Time.h>
#include <OGLEngine/Logger/Logger.h>

#include <math.h>
#include <iostream>
#include <GL/Glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
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

	glm::quat q = glm::normalize(gameobject->transform.GetGlobalRotation());
	float g_yaw = glm::yaw(q);
	glm::eulerAngles(q);
	float q0 = q.w;
	float q1 = q.x;
	float q2 = q.y;
	float q3 = q.z;
	float test_yaw = std::asin(2.0f * (q0 * q2 - q3 * q1));
	std::cout << test_yaw << " " << q.x << " " << q.y << " " << q.z << " " << q.w << std::endl;
	
	glm::vec3 dir(0.0f);
	float yaw = test_yaw;
	if (Input::IsKeyDown(GLFW_KEY_W)) {
		//glm::rotate(dir, q);
	}else if (Input::IsKeyDown(GLFW_KEY_S)) {
	}
	gameobject->transform.Translate(dir * speed);
	

	float turn_x = Time::delta_time * Input::delta_x;
	float turn_y = Time::delta_time * Input::delta_y;
	suppose_yaw += turn_x;

	glm::vec3 angle(0.0f, 1.0f, 0.0f);
	//gameobject->transform.Rotate(turn_x * 10.0f, angle);
	gameobject->transform.Rotate(glm::angleAxis(turn_x, glm::vec3(0.0f, 1.0f, 0.0f)));
}
void PlayerController::Draw() {
}