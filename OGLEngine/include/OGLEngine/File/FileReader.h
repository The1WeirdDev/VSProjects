#pragma once

#include "OGLEngine/Display/Mesh/BasicMesh.h"

namespace T1WD {
	class FileReader {
	public:
		static void Init();
		static void CleanUp();

		static void GenerateMeshFromFBXFile(const char* location, BasicMesh& mesh);
	private:
	};
}