#pragma once

#include "../Component/Component.h"

#include <glm/mat4x4.hpp>

namespace T1WD {
	class Camera : public Component {
	public:
		Camera();
		~Camera();

		void Awake() override;

		void Update() override;

		void CreateProjectionMatrix();

		float fov = 75.0f;
		float near = 0.01f;
		float far = 1000.0f;
		glm::mat4 projection_matrix;
		glm::mat4 transformation_matrix;
	};
}