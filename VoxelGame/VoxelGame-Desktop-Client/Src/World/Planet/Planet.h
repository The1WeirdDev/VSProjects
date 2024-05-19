#pragma once

#include <map>

#include "../Chunk/Chunk.h"

class Planet{
public:
	Planet();
	~Planet();

	void DeleteChunks();

	void SetName(const char* name) { this->name = name; }
	const char* GetName() { return name; }

	std::map<int, Chunk*> chunks;
private:
	const char* name;
};