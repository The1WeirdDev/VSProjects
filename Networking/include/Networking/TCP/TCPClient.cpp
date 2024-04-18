#include "TCPClient.h"
#include <iostream>
#include <array>

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
		std::array<char, 128> buf;

		asio::error_code error;
		size_t len = socket.read_some(asio::buffer(buf), error);

		if (error) {
			printf("ERROR READING\n");
			return;
		}

		for (int i = 0; i < 128; i++) {
			printf("%d", buf[i]);
		}
	}
}