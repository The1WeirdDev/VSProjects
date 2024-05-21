#pragma once
#include "OGLEngine/Object/Component/Component.h"
#include "OGLEngine/Display/Mesh/BasicMesh.h"

namespace T1WD {
	class UI : public Component {
	public:
		static void Init();
		static void CleanUp();

		static BasicMesh square_mesh;
	public:
		UI();
		~UI();

		//Z index will be 0-1000
		//Higher the z index further back
		int z_index = 0;
		bool is_visible = true;
	};
}