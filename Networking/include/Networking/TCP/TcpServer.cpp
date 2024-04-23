#include "TcpServer.h"
#include <iostream>

#include <Networking/Packet.h>

using asio::ip::tcp;

int TCPServer::port = 8888;
std::vector<TCPConnection*> TCPServer::connections;

#ifdef NETWORKING_EXPORTS
asio::io_service TCPServer::io_service;
asio::ip::tcp::acceptor* TCPServer::acceptor;
asio::ip::tcp::socket* TCPServer::socket;
#endif

void TCPServer::Start(int port) {
	TCPServer::port = port;

	acceptor = new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), port));
	printf("Started server on port %d\n", port);

	connections.clear();

	StartAccept();
	io_service.poll();
}

void TCPServer::Tick() {
	io_service.poll();
}
void TCPServer::StartAccept() {
	socket = new tcp::socket(io_service);
	acceptor->async_accept(*socket, &TCPServer::OnAccept);
}

void TCPServer::OnAccept(const asio::error_code& e) {
	Packet packet;
	int i = rand();
	std::string string = "Welcome. from server. " + std::to_string(i);
	packet.WriteString(string.c_str());
	packet.WriteLength();

	std::string message = "Hi\n";
	asio::error_code ignored_error;

	TCPConnection* connection = new TCPConnection(socket);
	connections.push_back(connection);
	//asio::write(socket, asio::buffer(packet.GetData(), 1024), ignored_error);
	asio::async_write(*connection->socket, asio::buffer(packet.GetData(), packet.packet_size), &TCPServer::OnWrite);
	StartAccept();
}
void TCPServer::OnWrite(const asio::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		printf("Error writing to tcp clients.\n");
	}
	else {
		printf("Transferred %d bytes.\n", (int)bytes_transferred);
	}
}