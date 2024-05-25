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

		//Position
		void SetPosition(glm::vec3 position);
		void SetGlobalPosition(glm::vec3 position);
		void Translate(glm::vec3 position);
		void OnPositionUpdate();

		//Rotation
		void SetRotation(glm::quat rotation);
		void SetGlobalRotation(glm::quat rotation);
		void Rotate(glm::quat rotation);
		void Rotate(float angle, glm::vec3& axis);
		void OnRotationUpdate();

		//Scale
		void SetScale(glm::vec3 scale);
		void SetGlobalScale(glm::vec3 scale);
		void Scale(glm::vec3 scale);
		void OnScaleUpdate();

		/*
		If you are to modify these values with a reference be aware you will have to call the transforms On___Update();
		*/
		glm::vec3& GetPosition() { return position; }
		glm::vec3& GetGlobalPosition() { return global_position; }

		glm::quat& GetRotation() { return rotation; }
		glm::quat& GetGlobalRotation() { return global_rotation; }

		glm::vec3& GetScale() { return scale; }
		glm::vec3& GetGlobalScale() { return global_scale; }

		glm::mat4& GetTransformationMatrix() { return transformation_matrix; }
		GameObject* gameobject = nullptr;
	private:
		void CalculateGlobalPosition();
		void CalculateGlobalRotation();
		void CalculateGlobalScale();

		void UpdateChildrenTransformations();

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		glm::vec3 global_position;
		glm::quat global_rotation;
		glm::vec3 global_scale;

		glm::mat4 transformation_matrix;
	};
}