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
#include <OGLEngine/Time/Time.h>
#include <OGLEngine/Input/Input.h>

#include <Networking/TCP/TCPClient.h>

#include <asio.hpp>

using namespace T1WD;
using asio::ip::tcp;

int main(int argc, char** argv) {
	LibraryManager::InitializeGLFW();

	Display::Create(1280, 720, "VoxelGame");
	Display::CenterWindow();

	Font::Init();

	UIRenderer::Init();
	/*
	Frame frame;
	frame.SetSize(0.5f, 0.5f);
	frame.screen_size_restraint = ScreenSizeRestraint::InverseAspectX;

	TextLabel text_label;
	text_label.SetText("HELLO WORLD");
	text_label.GenerateMesh();
	*/
	TCPClient client;

	client.on_connected = []() {
		printf("Connected to server.\n");
	};
	client.on_connect_failed = [](std::string message) {
		printf("Failed to connect to server. Reason ");
		std::cout << message << std::endl;
	};
	client.on_disconnected = []() {
		printf("Disconnected from server.\n");
	};

	client.on_packet_read = [](Packet* packet, size_t bytes_transferred) {
		std::cout << "Packet says " << packet->GetString() << std::endl;
	};
	client.Connect("192.168.0.38", 8888);
	std::thread t{[&client]() { client.Run(); }};

	Time::Init();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	printf("Initialized\n");
	while (Display::ShouldUpdateWindow()) {
		Display::PollEvents();

		//std::cout << (Input::IsKeyDown(GLFW_KEY_W) ? "true" : "false") << std::endl;
		Time::Update();
		//frame.y -= 0.001f;
		Display::ClearColors();

		Display::ClearDepth();
		//UIRenderer::RenderFrame(frame);
		//UIRenderer::RenderTextLabel(text_label);
		Display::SwapBuffers();
	}

	Display::Destroy();
	LibraryManager::TerminateGLFW();
	client.Disconnect();
	client.Stop();
	t.join();
	printf("Exited Program\n");
}