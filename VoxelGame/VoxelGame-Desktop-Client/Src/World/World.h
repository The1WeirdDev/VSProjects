#pragma once

#include <map>
#include <array>

#include "Planet/Planet.h"

class World{
public:
	World();
	~World();

	void Init();
	void CleanUp();

	void Update();

	void Draw();

	std::array<Planet*, 1> planets;
};