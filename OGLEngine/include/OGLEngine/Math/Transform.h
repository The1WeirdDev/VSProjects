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

		void SetScale(glm::vec3 scale);
		void SetGlobalScale(glm::vec3 scale);
		void Scale(glm::vec3 scale);
		void OnScaleUpdate();

		glm::vec3& GetPosition() { return position; }
		glm::vec3& GetGlobalPosition() { return global_position; }
		glm::vec3& GetScale() { return scale; }
		glm::vec3& GetGlobalScale() { return global_scale; }
		glm::mat4& GetTransformationMatrix() { return transformation_matrix; }
		GameObject* gameobject = nullptr;
	private:
		glm::vec3 CalculateGlobalPosition();
		glm::vec3 CalculateGlobalScale();

		void UpdateChildrenTransformations();

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::vec3 global_position;
		glm::vec3 global_scale;

		glm::mat4 transformation_matrix;
	};
}