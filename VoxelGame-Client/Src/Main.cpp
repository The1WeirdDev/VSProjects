#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <OGLEngine/Display/Shader/Shader.h>
#include <OGLEngine/UI/UIS/Frame.h>
#include <OGLEngine/Display/Rendering/UIRenderer.h>
#include <OGLEngine/Display/Font/Font.h>
#include <OGLEngine/Display/Display.h>
#include <OGLEngine/Display/Mesh/UITexturedMesh.h>
#include <OGLEngine/LibraryManager.h>
int main(int argc, char** argv) {
	LibraryManager::InitializeGLFW();

	Display::CreateWindow(1280, 720, "VoxelGame");
	Display::CenterWindow();

	Font::Init();

	UIRenderer::Init();
	Frame frame;
	frame.SetSize(0.5f, 0.5f);
	frame.screen_size_restraint = ScreenSizeRestraint::InverseAspectX;

	TextLabel text_label;
	text_label.SetText("HELLO WORLD");
	text_label.GenerateMesh();

	printf("Initialized\n");
	while (Display::ShouldUpdateWindow()) {
		Display::PollEvents();
		//frame.x -= 0.001f;
		//frame.y -= 0.001f;
		Display::ClearColors();

		Display::ClearDepth();
		UIRenderer::RenderFrame(frame);
		UIRenderer::RenderTextLabel(text_label);
		Display::SwapBuffers();
	}
	Display::Destroy();
	LibraryManager::TerminateGLFW();
	printf("Exited Program\n");
}