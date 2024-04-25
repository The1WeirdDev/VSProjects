#include <iostream>
#include <string>

#include <Networking/TCP/TCPServer.h>
#include <Networking/Packet.h>

using namespace T1WD;

int main(int argc, char** argv) {
	Packet p;
	TCPServer::on_server_started = []() {
		printf("Started Server\n");
	};
	TCPServer::on_server_start_failed = [](std::error_code ec) {
		printf("Failed to  Server\n");
	};
	TCPServer::on_client_connected = []() {
		printf("CLIENT CONNECTED\n");
	};
	TCPServer::Start(8888);

	while (true) {
		TCPServer::Tick();
	}
}