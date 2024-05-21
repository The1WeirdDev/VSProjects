#pragma once

#include <vector>
#include "OGLEngine/OGLEngine.h"

#include <glm/mat4x4.hpp>

namespace T1WD {
	extern "C" OGLENGINE_API class Shader {
	public:
		Shader();
		~Shader();

		OGLENGINE_API void Start();
		static OGLENGINE_API void Stop();
		void CleanUp();

		OGLENGINE_API void BindAttribute(int position, const char* name);
		OGLENGINE_API int GetUniformLocation(const char* name);

		//Adds it to the vector of uniforms and returns its index in the array
		OGLENGINE_API int AddUniform(const char* name);
		OGLENGINE_API int Get(int index);

		OGLENGINE_API void CreateShader(const char* vertex_shader_source, const char* fragment_shader_source);
		static OGLENGINE_API int CreateShader(const char* shader_data, int shader_type);

#pragma region 
		OGLENGINE_API void LoadVec2(int location, float x, float y);
		OGLENGINE_API void LoadVec3(int location, float x, float y, float z);
		OGLENGINE_API void LoadMat4x4(int location, glm::mat4x4& matrix);
#pragma endregion

		std::vector<int> uniforms;
	private:
		int vertex_shader_id = 0;
		int fragment_shader_id = 0;
		int program_id = 0;
		bool is_created = false;
	};
}