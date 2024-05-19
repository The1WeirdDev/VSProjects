#pragma once

#include "Mesh.h"

namespace T1WD {
	class Basic3DMesh : public Mesh {
	public:
		Basic3DMesh();
		~Basic3DMesh();

		void Create(float* vertex_data, size_t vertex_count, unsigned int* index_data, size_t index_count);

		void Draw() override;
		void CleanUp() override;

		unsigned int ebo_id = 0;
	};
}