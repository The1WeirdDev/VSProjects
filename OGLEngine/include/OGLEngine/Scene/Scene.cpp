#include "Scene.h"

using namespace T1WD;

Scene::Scene() {

}

Scene::~Scene() {

}

GameObject* Scene::CreateGameObject() {
	GameObject* gameobject = new GameObject();
	gameobject->scene = this;
	this->gameobjects.push_back(gameobject);
	return gameobject;
}

void Scene::Awake() {}
void Scene::Start() {}
void Scene::Update() {
	for (GameObject* gameobject : gameobjects) {
		gameobject->Update();
	}
}
void Scene::Draw() {
	for (GameObject* gameobject : gameobjects) {
		gameobject->Draw();
	}
}

void Scene::LateDraw(){
	for (GameObject* gameobject : gameobjects) {
		gameobject->LateDraw();
	}
}
void Scene::CleanUp() {
	for (GameObject* gameobject : gameobjects) {
		gameobject->CleanUp();
		delete gameobject;
	}

	gameobjects.resize(0);
}