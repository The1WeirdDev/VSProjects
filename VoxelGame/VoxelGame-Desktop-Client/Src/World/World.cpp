#include "World.h"

World::World() {
	Init();
}
World::~World() {
	CleanUp();
}
void World::Init() {
	Planet* earth = new Planet();
	earth->SetName("Earth");
	planets[0] = earth;
}
void World::CleanUp() {
	for (size_t i = 0; i < planets.size(); i++) {
		delete planets[i];
	}
}
void World::Update() {}

void World::Draw() {}