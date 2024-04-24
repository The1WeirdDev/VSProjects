#include "TcpServer.h"
#include <iostream>

#include <Networking/Packet.h>

#include <asio.hpp>

using asio::ip::tcp;


class TCPConnection {
public:
	TCPConnection(tcp::socket* socket) { this->socket = socket; }
	tcp::socket* socket;
};

int TCPServer::port = 8888;
std::vector<TCPConnection*> TCPServer::connections;

void* TCPServer::io_context;
void* TCPServer::acceptor;
void* TCPServer::socket;

void TCPServer::Start(int port) {
	TCPServer::port = port;

	io_context = new asio::io_context();
	acceptor = new tcp::acceptor(*(asio::io_context*)io_context, tcp::endpoint(tcp::v4(), port));

	printf("Started server on port %d\n", port);

	connections.clear();

	StartAccept();
	((asio::io_context*)io_context)->poll();
}

void TCPServer::Tick() {
	((asio::io_context*)io_context)->poll();
}
void TCPServer::StartAccept() {
	socket = new tcp::socket(*(asio::io_context*)io_context);
	((tcp::acceptor*)acceptor)->async_accept(*((tcp::socket*)socket), &TCPServer::OnAccept);
}

void TCPServer::OnAccept(const std::error_code& e) {
	Packet packet;
	int i = rand();
	std::string string = "Welcome. from server. " + std::to_string(i);
	packet.WriteString(string.c_str());
	packet.WriteLength();

	std::string message = "Hi\n";
	asio::error_code ignored_error;

	TCPConnection* connection = new TCPConnection((tcp::socket*)socket);
	connections.push_back(connection);

	unsigned char* data = packet.GetData();
	asio::async_write(*((tcp::socket*)socket), asio::buffer(packet.GetData(), packet.GetUsedSize()), [data](const asio::error_code& error, std::size_t bytes_transferred) {
		delete data;
		OnWrite(error, bytes_transferred);
	});
	StartAccept();
}
void TCPServer::OnWrite(const std::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		printf("Error writing to tcp clients.\n");
	}
	else {
		printf("Transferred %d bytes.\n", (int)bytes_transferred);
	}
}