#pragma once
#include "../Component/Component.h"

#include "OGLEngine/Display/Mesh/BasicMesh.h"

namespace T1WD {
	class BasicMeshRenderer : public Component {
	public:
		BasicMeshRenderer();
		~BasicMeshRenderer();

		void Awake() override;

		void Draw() override;

		BasicMesh mesh;
	};
}