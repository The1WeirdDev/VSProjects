#pragma once

#include "../Component/Component.h"

#include "OGLEngine/Display/Mesh/BasicMesh.h"

//This class acts as a mesh renderer EXCEPT it expects an existing mesh
//This takes a pointer to another mesh
//This will not cleanup the other mesh only use its resources
//The point of this is to use the same mesh but in different locations/gameobjects
namespace T1WD {
	class BasicMeshImpersonator : public Component {
	public:
		BasicMeshImpersonator();
		~BasicMeshImpersonator();

		void SetMesh(BasicMesh* mesh);

		void Draw() override;

	private:
		BasicMesh* mesh = nullptr;
	};
}