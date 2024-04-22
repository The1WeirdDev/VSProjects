#include "TCPClient.h"
#include <iostream>
#include <array>

#include <Networking/Packet.h>


using asio::ip::tcp;

TCPClient::TCPClient() : socket(io_context){
}
TCPClient::~TCPClient(){
	socket.cancel();
	io_context.stop();
}
void TCPClient::Connect(const char* ip, int port) {
	tcp::resolver resolver(io_context);

	_endpoints = resolver.resolve(ip, std::to_string(port));
	asio::async_connect(socket, _endpoints, [this](asio::error_code ec, asio::ip::tcp::endpoint ep) {
		OnConnect(ec);
	});

	io_context.run();
}

void TCPClient::Disconnect() {
	try {
		asio::error_code ec;
		socket.close(ec);
		io_context.stop();

		if (ec) {
			// process error
		}
	}
	catch (std::exception& e) {
		printf("Exception closing socket %s\n", e.what());
	}
}
void TCPClient::AsyncRead() {
	//socket->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), std::bind(&TCPClient::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	socket.async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), [this](asio::error_code ec, size_t bytes_transferred) {
		OnRead(ec, bytes_transferred);
	});
}
void TCPClient::OnConnect(const asio::error_code& e) {
	if (e) {
		printf("Failed to connect to server. Error %s\n", e.message().c_str());
		Disconnect();
		return;
	}

	printf("CONNECTED\n");
	AsyncRead();
}
void TCPClient::OnRead(const asio::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		Disconnect();
		printf("Error reading. %d bytes transferred.\n", (int)bytes_transferred);
	}
	else {
		printf("Read %d bytes\n", (int)bytes_transferred);
		Packet packet(NETWORKING_PACKET_SIZE, this->read_buffer.data());
		int length = packet.GetInt();
		std::cout << packet.GetString() << std::endl;

		AsyncRead();
	}
}