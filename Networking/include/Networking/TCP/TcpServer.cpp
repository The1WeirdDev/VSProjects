#include "TcpServer.h"
#include <iostream>

using asio::ip::tcp;

TCPServer::TCPServer() {

}
TCPServer::TCPServer(int port) {
	Start(port);
}

void TCPServer::Start(int port) {
	this->port = port;

	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
	printf("Started server on port %d\n", port);

	for (;;) {
		tcp::socket socket(io_service);
		acceptor.accept(socket);
		printf("Accepted\n");

		unsigned char* bytes = new unsigned char[1024];
		bytes[0] = 15;
		bytes[1] = 2;
		std::string message = "Hi\n";
		asio::error_code ignored_error;
		asio::write(socket, asio::buffer(bytes, 1024), ignored_error);

		while (true) {

		}
	}
}