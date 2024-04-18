#include "TcpServer.h"
#include <iostream>

using asio::ip::tcp;

TcpServer::TcpServer() {

}
TcpServer::TcpServer(int port) {
	Start(port);
}

void TcpServer::Start(int port) {
	this->port = port;

	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
	printf("Started server on port %d\n", port);

	for (;;) {
		tcp::socket socket(io_service);
		acceptor.accept(socket);
		printf("Accepted\n");
		std::string message = "HELLO\n";
		while (message.size() < 128)message += " ";
		asio::error_code ignored_error;
		asio::write(socket, asio::buffer(message), ignored_error);
	}
}