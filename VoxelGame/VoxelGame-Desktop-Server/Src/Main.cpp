#include <iostream>
#include <string>

#define NETWORKING_READ_SAFE
#define NETWORKING_STATIC_BUILD
#include <Networking/TCP/TCPServer.h>

#include <Networking/Packet.h>

using namespace T1WD;

int main(int argc, char** argv) {
	TCPServer server;
	server.on_server_started = []() {
		printf("Started Server\n");
	};
	server.on_server_stopped = [](bool error, const char* message) {
		if (error) {
			printf("Server stopped because of %s\n", message);
		}
		else {
			printf("Stopped server\n");
		}
	};
	server.on_server_start_failed = [](std::error_code ec) {
		printf("Failed to start Server\n");
	};
	server.on_client_connected = [&server](unsigned short id) {
		printf("CLIENT %d CONNECTED\n", id);
		Packet* packet = new Packet();
		packet->WriteString((std::string("HELLO Client ") + std::to_string(id)));
		packet->WriteFloat(151.213123123);
		packet->delete_data = true;
		server.SendPacket(id, packet, false);
		server.SendPacket(id, packet, false);
		server.SendPacket(id, packet, false);
		server.SendPacket(id, packet, true);
	};
	server.on_client_failed_connect = [](const char* message) {
		printf("CLIENT failed to connect. Reason %s\n", message);
	};
	server.on_client_disconnected = [](unsigned short id) {
		printf("CLIENT %d Disconnected\n", id);
	};
	server.on_packet_read = [](unsigned short id, Packet* packet, size_t bytes_transferred) {
		std::cout << "PACKET SAID " << packet->GetString() << std::endl;
		size_t size = 0;
		unsigned char* data = packet->GetUCharArray(&size);

		std::cout << "SIZE: " << size << std::endl;
		for (int i = 0; i < size; i++) {
			printf("%d\n", (int)data[i]);
		}
	};
	server.Start(8888);

	while (true) {
		server.Tick();
	}
}