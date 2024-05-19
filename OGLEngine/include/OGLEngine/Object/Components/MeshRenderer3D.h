#pragma once
#include "../Component/Component.h"

#include "OGLEngine/Display/Mesh/Basic3DMesh.h"

namespace T1WD {
	class MeshRenderer3D : public Component {
	public:
		MeshRenderer3D();
		~MeshRenderer3D();

		void Awake() override;

		void Draw() override;

		Basic3DMesh mesh;
	};
}