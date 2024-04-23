#include "TCPClient.h"
#include <iostream>
#include <array>

#include <Networking/Packet.h>


using asio::ip::tcp;

TCPClient::TCPClient(){
	socket = new tcp::socket(io_context);
}
TCPClient::~TCPClient(){
	if (socket != nullptr) {
		Disconnect();
		delete socket;
	}
}
void TCPClient::Connect(const char* ip, int port) {
	tcp::resolver resolver(io_context);

	_endpoints = resolver.resolve(ip, std::to_string(port));
	is_attempting_connect = true;
	asio::async_connect(*socket, _endpoints, [this](asio::error_code ec, asio::ip::tcp::endpoint ep) {
		OnConnect(ec);
	});
}

void TCPClient::Disconnect() {
	try {
		/*
		printf("Attempting Disconnect\n");
		if (is_attempting_connect || is_connected) {
			printf("Calling Disconnect\n");
			if(socket->is_open())
				socket->shutdown(socket->shutdown_both);
		}
		*/
		asio::error_code ec;
		socket->close(ec);

		if (ec) {
			printf("Error closing socket %s\n", ec.message());
		}
	}
	catch (std::exception& e) {
		printf("Exception closing socket %s\n", e.what());
	}
}

void TCPClient::Run() {
	try {
		io_context.run();
		printf("HELO\n");
	}
	catch (std::exception& e) {
		printf("Error with io context %s\n", e.what());
	}
}
void TCPClient::AsyncRead() {
	//socket->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), std::bind(&TCPClient::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	socket->async_read_some(asio::buffer(this->read_buffer), [this](asio::error_code ec, size_t bytes_transferred) {
		OnRead(ec, bytes_transferred);
	});
}
void TCPClient::OnConnect(const asio::error_code& e) {
	is_attempting_connect = false;
	if (e) {
		printf("Failed to connect to server. Error %s\n", e.message().c_str());
		Disconnect();
		return;
	}

	is_connected = true;

	printf("CONNECTED\n");
	AsyncRead();
}
void TCPClient::OnRead(const asio::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		printf("Error reading\n");
		Disconnect();
		return;
	}

	printf("Read %d bytes\n", (int)bytes_transferred);
	Packet packet(NETWORKING_PACKET_SIZE, this->read_buffer.data());
	int length = packet.GetInt();
	std::cout << packet.GetString() << std::endl;

	AsyncRead();
}