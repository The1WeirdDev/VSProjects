#pragma once

#include <iostream>
#include <vector>

#include <glm/vec3.hpp>

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

		void Awake();
		void Start();
		void CleanUp();

		void Update();
		void Draw();
		void LateDraw();

		void SetPosition(glm::vec3 position);
		void SetGlobalPosition(glm::vec3 position);

		glm::vec3 GetPosition() { return position; }
		glm::vec3 GetGlobalPosition() { return global_position; }

		std::vector<Component*> components;
		std::vector<GameObject*> children;
		GameObject* parent = nullptr;
		Scene* scene = nullptr;
	private:
		glm::vec3 position;	//Relative Position
		glm::vec3 global_position;
	};
}