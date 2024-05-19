#include "Camera.h"

#include <iostream>

#include "../GameObject.h"
#include "../../Scene/Scene.h"

#include "OGLEngine/Display/Display.h"

#include <glm/gtc/matrix_transform.hpp>

#include "OGLEngine/Display/Shader/Shaders.h"

using namespace T1WD;
Camera::Camera() : Component(){
	name = "Camera";
}
Camera::~Camera() {

}

void Camera::Awake() {
	if (gameobject->scene->camera == nullptr) {
		gameobject->scene->camera = this;

		projection_matrix = glm::perspective(glm::radians(fov), Display::aspect_ratio, near, far);
		transformation_matrix = glm::mat4(1.0f);
		return;
	}

	printf("Multiple camera in one scene\n");
}

void Camera::Update() {
	Shaders::basic_mesh_3d_shader.Start();
	Shaders::basic_mesh_3d_shader.LoadMat4x4(Shaders::basic_mesh_3d_shader.uniforms[0], projection_matrix);
	//glm::vec3 pos = gameobject->GetGlobalPosition();

}
void Camera::CreateProjectionMatrix() {
	projection_matrix = glm::perspective(glm::radians(fov), Display::aspect_ratio, near, far);
}