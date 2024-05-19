#pragma once

#include <iostream>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace T1WD {
	class Component;
	class Scene;
	class GameObject {
	public:
		GameObject();
		~GameObject();

		//Add components made on the heap unless it has a longer alive time
		//Always pass a new component into this
		GameObject* CreateChild();
		Component* AddComponent(Component* component);
		Component* GetComponent(const char* component_name);

		void Awake();
		void Start();
		void CleanUp();

		void Update();
		void Draw();
		void LateDraw();

		void SetPosition(glm::vec3 position);
		void SetGlobalPosition(glm::vec3 position);
		void Translate(glm::vec3 position);


		glm::vec3& GetPosition() { return position; }
		glm::vec3& GetGlobalPosition() { return global_position; }
		glm::vec3 GetParentsGlobalPosition();

		glm::mat4& GetTransformationMatrix() { return transformation_matrix; }

		std::vector<Component*> components;
		std::vector<GameObject*> children;
		GameObject* parent = nullptr;
		Scene* scene = nullptr;
	private:
		glm::vec3 CalculateGlobalPosition();

		void UpdateChildrenTransformations();
		void GenerateTransformationMatrix();
	private:
		glm::vec3 position;	//Relative Position
		glm::vec3 global_position;

		glm::mat4 transformation_matrix;
	};
}