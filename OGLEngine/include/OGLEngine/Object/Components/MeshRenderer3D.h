#pragma once
#include "../Component/Component.h"

#include "OGLEngine/Display/Mesh/BasicMesh.h"

namespace T1WD {
	class MeshRenderer3D : public Component {
	public:
		MeshRenderer3D();
		~MeshRenderer3D();

		void Awake() override;

		void Draw() override;

		BasicMesh mesh;
	};
}