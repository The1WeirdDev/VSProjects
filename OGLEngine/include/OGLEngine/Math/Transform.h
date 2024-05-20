#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace T1WD {
	class GameObject;
	class Transform {
	public:
		Transform();
		~Transform();

		void GenerateTransformationMatrix();

		void SetPosition(glm::vec3 position);
		void SetGlobalPosition(glm::vec3 position);
		void Translate(glm::vec3 position);

		glm::vec3& GetPosition() { return position; }
		glm::vec3& GetGlobalPosition() { return global_position; }
		glm::mat4& GetTransformationMatrix() { return transformation_matrix; }
		GameObject* gameobject;
	private:
		glm::vec3 CalculateGlobalPosition();

		void UpdateChildrenTransformations();
		void GenerateTransformationMatrix();

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::vec3 global_position;

		glm::mat4 transformation_matrix;
	};
}