#pragma once

#include <iostream>
#include <vector>

#include "OGLEngine/Math/Transform.h"

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

		void SetScale(glm::vec3 position);
		void SetGlobalScale(glm::vec3 position);
		void Scale(glm::vec3 scale);

		glm::vec3& GetPosition() { return transform.GetPosition(); }
		glm::vec3& GetGlobalPosition() { return transform.GetGlobalPosition(); }

		glm::quat& GetRotation() { return transform.GetRotation(); }
		glm::quat& GetGlobalRotation() { return transform.GetGlobalRotation(); }

		glm::vec3& GetScale() { return transform.GetScale(); }
		glm::vec3& GetGlobalScale() { return transform.GetGlobalScale(); }

		glm::mat4& GetTransformationMatrix() { return transform.GetTransformationMatrix(); }

		std::vector<Component*> components;
		std::vector<GameObject*> children;
		GameObject* parent = nullptr;
		Scene* scene = nullptr;
		Transform transform;
	private:
	};
}