#pragma once

#include <vector>

#include "../Object/GameObject.h"

namespace T1WD {
#ifndef Camera
	class Camera;
#endif
	class Scene {
	public:
		Scene();
		~Scene();

		//Intended to be called after Scene Awake and Start
		GameObject* CreateGameObject();

		void Awake();	//Called when scene is loaded
		void Start();	//Called aftter Awake

		void Update();

		void Draw();
		void LateDraw();	//Meant for Drawing UI

		void CleanUp();

		Camera* camera = nullptr;
	private:
		std::vector<GameObject*> gameobjects;
	};
}
