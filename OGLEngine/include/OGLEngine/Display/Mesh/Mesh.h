#pragma once

namespace T1WD {
	class Mesh {
	public:
		Mesh();
		~Mesh();

		virtual void Draw() {}
		virtual void CleanUp() {}

		unsigned int vao_id = 0;
		unsigned int vbo_id = 0;
		unsigned int index_count = 0;
		bool is_created = false;
	};
}