#include <iostream>

#define OGLENGINE_STATIC
#define NETWORKING_STATIC_BUILD
#include <OGLEngine/Display/Shader/Shader.h>
#include <OGLEngine/UI/UIS/Frame.h>
#include <OGLEngine/Display/Rendering/UIRenderer.h>
#include <OGLEngine/Display/Font/Font.h>
#include <OGLEngine/Display/Display.h>
#include <OGLEngine/Display/Mesh/UITexturedMesh.h>
#include <OGLEngine/Display/Shader/Shaders.h>
#include <OGLEngine/LibraryManager.h>
#include <OGLEngine/Time/Time.h>
#include <OGLEngine/Input/Input.h>
#include <OGLEngine/Scene/Scene.h>
#include <OGLEngine/Object/Component/Component.h>
#include <OGLEngine/Object/Components/Camera.h>
#include <OGLEngine/Object/Components/MeshRenderer3D.h>

#include <Networking/TCP/TCPClient.h>

#include "Player/PlayerController.h"

#include <glm/vec3.hpp>

using namespace T1WD;
//#include "Game/World.h"

//using namespace T1WD;
int main(int argc, char** argv) {
	LibraryManager::InitializeGLFW();

	Display::Create(1280, 720, "VoxelGame");
	Display::CenterWindow();

	UIRenderer::Init();
	//World* world = nullptr;

	Frame frame;
	frame.SetSize(0.5f, 0.5f);
	frame.screen_size_restraint = ScreenSizeRestraint::InverseAspectX;

	TextLabel text_label;
	text_label.SetText("HELLO WORLD");
	text_label.GenerateMesh();
	text_label.screen_size_restraint = ScreenSizeRestraint::InverseAspectX;
	text_label.width = 0.5;
	text_label.height = 0.5;
	//TCPServer::Start(8888);

	TCPClient client;

	client.on_connected = []() {
		printf("Connected to server.\n");
		//world = new World();
		};
	client.on_connect_failed = [](std::error_code error) {
		printf("Failed to connect to server. Reason ");
		std::cout << error.message() << std::endl;
		};
	client.on_disconnected = [](std::error_code& error) {
		printf("Disconnected from server.");
	};

	client.on_packet_read = [](Packet* packet, size_t bytes_transferred) {
		packet->GetString();
		std::cout << "Packet says " << packet->GetFloat() << std::endl;
		};
	const char* ip = "10.16.32.13";
	client.Connect(ip, 8888);

	//std::thread t{ [&client]() { client.Run(); } };
	Shaders::Init();
	Time::Init();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Scene scene;
	scene.Awake();
	scene.Start();

	float* vertices = new float[12] {
		0, 0, -5,
			0, 1, -5,
			1, 0, -5,
			1, 1, -5
		};
	unsigned int* indices = new unsigned int[6] {
			0, 1, 2, 2, 1, 3
			};
	GameObject* mesh_object = scene.CreateGameObject();
	MeshRenderer3D* mesh_renderer = (MeshRenderer3D*)mesh_object->AddComponent(new MeshRenderer3D());

	GameObject* player = scene.CreateGameObject();
	player->AddComponent(new Camera());
	player->AddComponent(new PlayerController());


	mesh_renderer->mesh.Create(vertices, 12, indices, 6);
	glClearColor(0, 0.8f, 1.0f, 1.0f);
	Display::SetSwapInterval(0);
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
		//object->Translate(glm::vec3(0, 0, -1 * Time::delta_time));
		//glm::vec3& pos = child->GetGlobalPosition();
		//std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
		if (Input::IsKeyPressed(GLFW_KEY_G)) {
			if (client.is_connected || client.is_attempting_connect)client.Disconnect();
			//client.Connect(ip, 8888);
		}

		if (Input::IsKeyPressed(GLFW_KEY_H)) {
			client.Connect(ip, 8888);
		}

		Time::Update();
		scene.Update();
		Input::Update();

		Display::ClearColors();
		scene.Draw();

		Display::ClearDepth();
		scene.LateDraw();
		UIRenderer::RenderTextLabel(text_label);
		Display::SwapBuffers();
	}

	scene.CleanUp();

	Shaders::CleanUp();
	Display::Destroy();
	client.Disconnect();
	client.Stop();
	LibraryManager::TerminateGLFW();
	printf("Exited Program\n");
}