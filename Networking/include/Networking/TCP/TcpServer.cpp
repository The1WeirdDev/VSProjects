#include "TcpServer.h"
#include <iostream>

#include <Networking/Packet.h>

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

		Packet packet;
		packet.WriteString("Welcome. from server.");

		std::string message = "Hi\n";
		asio::error_code ignored_error;
		//asio::write(socket, asio::buffer(packet.GetData(), 1024), ignored_error);
		asio::async_write(socket, asio::buffer(packet.GetData(), 1024), &TCPServer::OnWrite);
		while (true) {

		}
	}
}
void TCPServer::OnWrite(const asio::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		printf("Error writing to tcp clients.\n");
	}
	else {
		printf("Transferred %d bytes.\n", (int)bytes_transferred);
	}
}