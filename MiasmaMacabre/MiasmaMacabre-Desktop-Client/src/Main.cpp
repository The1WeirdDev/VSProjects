#include <iostream>

#define OGLENGINE_STATIC
#define NETWORKING_STATIC_BUILD

#define GLM_FORCE_RADIANS 

#include <OGLEngine/Display/Shader/Shader.h>
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
#include <OGLEngine/Object/Components/BasicMeshRenderer.h>
#include <OGLEngine/Object/Components/BasicMeshImpersonator.h>
#include <OGLEngine/Object/Components/UI/Frame.h>
#include <OGLEngine/File/FileReader.h>

#include <Networking/TCP/TCPClient.h>

#include "Player/PlayerController.h"

#include <glm/vec3.hpp>

using namespace T1WD;
#ifdef RELEASE_BUILD
#include <windows.h>
#define _WIN32_WINNT_WIN10                  0x0A00 // Windows 10
int WINAPI wWinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	PWSTR       lpCmdLine,
	int         nCmdShow
){
#else
int main(int argc, char** argv){
#endif

	LibraryManager::InitializeGLFW();

	Display::Create(1280, 720, "VoxelGame");
	Display::SetBackgroundColor(0, 0.8f, 1.0f, 1.0f);
	Display::CenterWindow();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shaders::Init();
	UI::Init();
	Time::Init();
	FileReader::Init();

	Scene scene;
	scene.Awake();
	scene.Start();

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

	float* vertices = new float[15] {
		0, 0, 0,
			0, 1, 0,
			1, 0, 0,
			1, 1, 0,
			1,1,1
		};
	unsigned int* indices = new unsigned int[9] {
			0, 1, 2, 2, 1, 3,
				2,3, 4
			};
	GameObject* mesh_object = scene.CreateGameObject();
	BasicMeshRenderer* mesh_renderer = (BasicMeshRenderer*)mesh_object->AddComponent(new BasicMeshRenderer());
	GameObject* impersonator = mesh_object->CreateChild();
	BasicMeshImpersonator* mesh_impersonator = new BasicMeshImpersonator();
	mesh_impersonator->SetMesh(&mesh_renderer->mesh);
	impersonator->AddComponent(mesh_impersonator);
	GameObject* canvas = scene.CreateGameObject();
	canvas->AddComponent(new Frame());

	GameObject* player = scene.CreateGameObject();
	player->AddComponent(new Camera());
	player->AddComponent(new PlayerController());

	//mesh_renderer->mesh.Create(3, vertices, 15, indices, 9);
	mesh_renderer->gameobject->Translate(glm::vec3(0, 0, -5));

	FileReader::GenerateMeshFromFBXFile("res/test.fbx", mesh_renderer->mesh);
	mesh_object->Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	impersonator->Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	float angle = 0.0f;

	Display::SetSwapInterval(1);
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
			if (client.is_connected || client.is_attempting_connect)client.Disconnect();
		}

		if (Input::IsKeyPressed(GLFW_KEY_H)) {
			client.Connect(ip, 8888);
		}

		if (Input::IsKeyPressed(GLFW_KEY_I)) {
			Display::ToggleFullscreen();
		}

		angle += 2.0f * Time::delta_time;

		Time::Update();

		float amount = 1.01f * Time::delta_time;
		//impersonator->transform.SetRotation(glm::vec3(0, angle, 0));
		//scale.x += amount;
		///mesh_object->transform.OnScaleUpdate();

		canvas->Translate(glm::vec3(0.2 * Time::delta_time, 0, 0));
		scene.Update();
		Input::Update();

		Display::ClearColors();
		scene.Draw();

		Display::ClearDepth();
		scene.LateDraw();
		Display::SwapBuffers();
	}

	scene.CleanUp();
	client.Stop();
	UI::CleanUp();
	FileReader::CleanUp();
	Shaders::CleanUp();
	Display::Destroy();
	LibraryManager::TerminateGLFW();
	printf("Exited Program\n");
}