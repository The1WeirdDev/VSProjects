#pragma once

#include "Shader.h"

namespace T1WD {
	class Shaders {
	public:
		//Initialized shaders
		static void Init();
		static void CleanUp();

		static Shader basic_mesh_3d_shader;
	};
}