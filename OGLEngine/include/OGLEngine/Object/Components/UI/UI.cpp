#include "UI.h"

using namespace T1WD;

BasicMesh UI::square_mesh;
UI::UI() {
	name = "UI";
}
UI::~UI() {

}

void UI::Init() {
	float* vertices = new float[8] {
		0, 0,
			0, 1,
			1, 0,
			1, 1
		};

	unsigned int* indices = new unsigned int[6] {
		0, 1, 2, 2, 1, 3
		};
	square_mesh.Create(2, vertices, 8, indices, 6);
}

void UI::CleanUp() {
	square_mesh.CleanUp();
}