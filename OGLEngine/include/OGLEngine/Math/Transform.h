#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

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
		void OnPositionUpdate();

		void SetScale(glm::vec3 scale);
		void SetGlobalScale(glm::vec3 scale);
		void Scale(glm::vec3 scale);
		void OnScaleUpdate();

		/*
		If you are to modify these values with a reference be aware you will have to call the transforms On___Update();
		*/
		glm::vec3& GetPosition() { return position; }
		glm::vec3& GetGlobalPosition() { return global_position; }
		glm::vec3& GetScale() { return scale; }
		glm::vec3& GetGlobalScale() { return global_scale; }
		glm::mat4& GetTransformationMatrix() { return transformation_matrix; }
		GameObject* gameobject = nullptr;
	private:
		void CalculateGlobalPosition();
		void CalculateGlobalScale();

		void UpdateChildrenTransformations();

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		glm::vec3 global_position;
		glm::vec3 global_scale;

		glm::mat4 transformation_matrix;
	};
}