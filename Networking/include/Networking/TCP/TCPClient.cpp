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
	tcp::resolver resolver(io_service);

	tcp::resolver::query query(ip, "daytime");
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(ip),port);

	socket = new tcp::socket(io_service, ep.protocol());

	socket->async_connect(ep, std::bind(&TCPClient::OnConnect, this, std::placeholders::_1));
	io_service.run();
}

void TCPClient::Disconnect() {
	if (socket == nullptr)return;
	try {

		socket->cancel();
		socket->close();
		socket->shutdown(socket->shutdown_both);
		io_service.stop();
		delete socket;
		socket = nullptr;
	}
	catch (std::exception& e) {
		printf("Exception closing socket %s\n", e.what());
	}
}
void TCPClient::AsyncRead() {
	//socket->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), std::bind(&TCPClient::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	socket->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), [this](asio::error_code ec, size_t bytes_transferred) {
		OnRead(ec, bytes_transferred);
		});

	printf("TEST1\n");
}
void TCPClient::OnConnect(const asio::error_code& e) {
	if (e) {
		printf("Failed to connect to server.\n");
		return;
	}

	printf("CONNECTED\n");
	AsyncRead();
}
void TCPClient::OnRead(const asio::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		printf("Error reading. %d bytes transferred.\n", (int)bytes_transferred);
	}
	else {
		printf("Read %d bytes\n", (int)bytes_transferred);
		Packet packet(NETWORKING_PACKET_SIZE, this->read_buffer.data());
		std::cout << packet.GetString() << std::endl;

		AsyncRead();
	}
}