#pragma once

#include <vector>
#include "OGLEngine/OGLEngine.h"

namespace T1WD {
	extern "C" OGLENGINE_API class Shader {
	public:
		OGLENGINE_API Shader();
		OGLENGINE_API ~Shader();

		OGLENGINE_API void Start();
		static OGLENGINE_API void Stop();

		OGLENGINE_API void BindAttribute(int position, const char* name);
		OGLENGINE_API int GetUniformLocation(const char* name);

		OGLENGINE_API void CreateShader(const char* vertex_shader_source, const char* fragment_shader_source);
		static OGLENGINE_API int CreateShader(const char* shader_data, int shader_type);

#pragma region 
		OGLENGINE_API void LoadVec2(int location, float x, float y);
		OGLENGINE_API void LoadVec3(int location, float x, float y, float z);
#pragma endregion

		std::vector<int> uniforms;
	private:
		int vertex_shader_id = 0;
		int fragment_shader_id = 0;
		int program_id = 0;
	};
}