#include "TCPClient.h"

#include <iostream>
#include <array>

#include <Networking/Packet.h>

#include <asio.hpp>

using asio::ip::tcp;

TCPClient::TCPClient(){
	io_context = (void*)new asio::io_context();
	socket = (void*)new tcp::socket(*(asio::io_context*)io_context);
}
TCPClient::~TCPClient(){
	delete (tcp::socket*)socket;
	delete (asio::io_context*)io_context;
}
void TCPClient::Connect(const char* ip, int port){
	is_disconnecting = false;
	asio::io_context* context = (asio::io_context*)io_context;
	tcp::resolver resolver(*context);

	asio::ip::tcp::resolver::results_type _endpoints = resolver.resolve(ip, std::to_string(port));
	is_attempting_connect = true;
	is_connected = false;
	asio::async_connect(*((tcp::socket*)socket), _endpoints, [this](asio::error_code ec, asio::ip::tcp::endpoint ep) {
		OnConnect(nullptr, ec);
	});
}

void TCPClient::Disconnect() {
	try {
		if ((is_attempting_connect || is_connected) && !is_disconnecting) {
			is_disconnecting = true;
			asio::error_code ec;

			((tcp::socket*)socket)->cancel(ec);
			if (ec) {
				printf("Error cancelling socket %s\n", ec.message());
			}

			if (is_connected) {
				((tcp::socket*)socket)->shutdown(((tcp::socket*)socket)->shutdown_both);

				if (ec) {
					printf("Error shuttingdown socket %s\n", ec.message());
				}
			}
			((tcp::socket*)socket)->close(ec);

			if (ec) {
				printf("Error closing socket %s\n", ec.message());
			}
			if (!is_connected) {
				if (on_connect_failed)on_connect_failed(std::error_code(std::error_code::));
			}
			else {
				if (on_disconnected)on_disconnected();
			}
			is_attempting_connect = false;
			is_connected = false;
		}
	}
	catch (std::exception& e) {
		printf("Exception closing socket %s\n", e.what());
	}
}

void TCPClient::Run() {
	try {
		((asio::io_context*)io_context)->run();
	}
	catch (std::exception& e) {
		printf("Error with io context %s\n", e.what());
	}
}

void TCPClient::Stop() {
	return;
	/*
	try {
		io_context.stop();
	}
	catch (std::exception& e) {
		printf("Error stopping io_context : Error %s\n", e.what());
	}*/
}

//#ifdef NETWORKING_EXPORTS
void TCPClient::AsyncRead() {
	//socket->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), std::bind(&TCPClient::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	
	((tcp::socket*)socket)->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), [this](asio::error_code ec, size_t bytes_transferred) {
		if (is_disconnecting)return;
		OnRead(ec, bytes_transferred);
	});
}

void TCPClient::AsyncWrite() {
	Packet packet;
	unsigned char* data = packet.GetData();
	asio::async_write(*((tcp::socket*)socket), asio::buffer(packet.GetData(), packet.GetUsedSize()), [data](const asio::error_code& error, std::size_t bytes_transferred) {
		delete data;
		//OnWrite(error, bytes_transferred);
	});
}
void TCPClient::OnConnect(std::shared_ptr<TCPClient> seslf, const std::error_code& e) {
	if (is_disconnecting)return;

	if (e) {
		if (on_connect_failed)on_connect_failed(e);
		Disconnect();
		return;
	}

	is_connected = true;
	if (on_connected)on_connected();

	printf("CONNECTED\n");
	AsyncRead();
}
void TCPClient::OnRead(const std::error_code& error, std::size_t bytes_transferred) {
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
void TCPClient::OnWrite(const std::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		printf("Error writing\n");
		Disconnect();
		return;
	}

}
//#endif