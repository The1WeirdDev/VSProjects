#include <iostream>
#include <string>

#include <Networking/TCP/TCPServer.h>

#define NETWORKING_READ_SAFE
#include <Networking/Packet.h>


using namespace T1WD;

int main(int argc, char** argv) {
	TCPServer::on_server_started = []() {
		printf("Started Server\n");
	};
	TCPServer::on_server_stopped = [](bool error, const char* message) {
		if (error) {
			printf("Server stopped because of %s\n", message);
		}
		else {
			printf("Stopped server\n");
		}
	};
	TCPServer::on_server_start_failed = [](std::error_code ec) {
		printf("Failed to start Server\n");
	};
	TCPServer::on_client_connected = [](unsigned short id) {
		printf("CLIENT %d CONNECTED\n", id);
		Packet packet(500);
		packet.WriteString((std::string("HELLO Client ") + std::to_string(id)));
		TCPServer::SendPacket(id, &packet);
	};
	TCPServer::on_client_failed_connect = [](const char* message) {
		printf("CLIENT failed to connect. Reason %s\n", message);
	};
	TCPServer::on_client_disconnected = [](unsigned short id) {
		printf("CLIENT %d Disconnected\n", id);
	};
	TCPServer::on_packet_read = [](unsigned short id, Packet* packet, size_t bytes_transferred) {
		std::cout << "PACKET SAID " << packet->GetString() << std::endl;
		size_t size = 0;
		unsigned char* data = packet->GetUCharArray(&size);
		std::cout << "SIZE: " << size << std::endl;
		for (int i = 0; i < size; i++) {
			printf("%d\n", (int)data[i]);
		}
	};
	TCPServer::Start(8888);

	while (true) {
		TCPServer::Tick();
	}
}