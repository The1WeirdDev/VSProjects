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

	
	Frame frame;
	frame.SetSize(0.5f, 0.5f);
	frame.screen_size_restraint = ScreenSizeRestraint::InverseAspectX;
	/*
	TextLabel text_label;
	text_label.SetText("HELLO WORLD");
	text_label.GenerateMesh();
	*/
	TCPClient client;

	client.on_connected = []() {
		printf("Connected to server.\n");
	};
	client.on_connect_failed = [](std::error_code error) {
		printf("Failed to connect to server. Reason ");
		std::cout << error.message() << std::endl;
	};
	client.on_disconnected = [](std::error_code& error) {
		printf("Disconnected from server.");

		if (error)printf(" Reason %s", error.message());

		printf("\n");
	};

	client.on_packet_read = [](Packet* packet, size_t bytes_transferred) {
		packet->GetString();
		std::cout << "Packet says " << packet->GetFloat() << std::endl;
	};
	const char* ip = "10.16.34.119";
	client.Connect(ip, 8888);
	//std::thread t{ [&client]() { client.Run(); } };

	Time::Init();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	printf("Initialized\n");
	while (Display::ShouldUpdateWindow()) {
		Display::PollEvents();

		if (Input::IsKeyPressed(GLFW_KEY_R)) {
			if (client.is_connected) {
				printf("SENDING PACKET\n");
				Packet* packet = new Packet(500);
				packet->delete_data = true;
				packet->WriteString("Hello Server\n");
				unsigned char data[10];
				data[0] = 15;
				data[1] = 212;
				data[2] = 255;
				data[3] = 127;
				packet->WriteUCharArray(10, data);
				client.Post(packet);
			}
		}

		if (Input::IsKeyPressed(GLFW_KEY_G)) {
			if (client.is_connected)client.Disconnect();
			//client.Connect(ip, 8888);
		}

		if (Input::IsKeyPressed(GLFW_KEY_H)) {
			client.Connect(ip, 8888);
		}

		//std::cout << (Input::IsKeyDown(GLFW_KEY_W) ? "true" : "false") << std::endl;
		Time::Update();
		Input::Update();
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
	//t.join();
	printf("Exited Program\n");
}