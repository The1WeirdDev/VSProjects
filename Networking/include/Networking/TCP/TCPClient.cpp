#include "TCPClient.h"
#include <iostream>
#include <array>

#include <Networking/Packet.h>


using asio::ip::tcp;

TCPClient::TCPClient() {}
TCPClient::TCPClient(const char* ip, int port) {
	Connect(ip, port);
}

void TCPClient::Connect(const char* ip, int port) {
	printf("TEST1\n");
	tcp::resolver resolver(io_service);

	printf("TEST2\n");
	tcp::resolver::query query(ip, "daytime");
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(ip),port);

	printf("TEST3\n");
	tcp::socket socket(io_service, ep.protocol());
	printf("TEST4\n");
	socket.connect(ep);
	printf("CONNECTED\n");
	for (;;) {
		std::array<unsigned char, 1024> buf;

		asio::error_code error;
		size_t len = socket.read_some(asio::buffer(buf), error);

		Packet packet(1024, buf.data());

		if (error) {
			printf("ERROR READING\n");
			return;
		}

		std::cout << packet.GetString() << std::endl;
	}
}