#pragma once

#include "Mesh.h"

namespace T1WD {
	class BasicMesh : public Mesh {
	public:
		BasicMesh();
		~BasicMesh();

		void Create(unsigned char dimensions, float* vertex_data, size_t vertex_count, unsigned int* index_data, size_t index_count);

		void Draw() override;
		void CleanUp() override;

		unsigned int ebo_id = 0;
		unsigned char dimensions = 3;
	};
}