#pragma once
#include "OGLEngine/OGLEngine.h"

#include "Mesh.h"

namespace T1WD {
	extern "C" OGLENGINE_API class UITexturedMesh : public Mesh{
	public:
		OGLENGINE_API UITexturedMesh();

		OGLENGINE_API void Create(unsigned char* vertices, unsigned char vertex_count, float* texture_coords, unsigned char texture_coord_count, unsigned char* indices, unsigned char index_count);
		OGLENGINE_API void Draw();
	private:
		unsigned int tbo_id;
		unsigned int ebo_id;
	};
}