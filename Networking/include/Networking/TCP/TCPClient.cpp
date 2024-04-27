#include "TCPClient.h"

#include <iostream>
#include <array>

#include <Networking/Packet.h>
#include <Networking/ErrorCodes.h>

#include <asio.hpp>

using asio::ip::tcp;

namespace T1WD {

	TCPClient::TCPClient() {
		io_context = (void*)new asio::io_context();
		socket = (void*)new tcp::socket(*(asio::io_context*)io_context);
	}
	TCPClient::~TCPClient() {
		delete (tcp::socket*)socket;
		delete (asio::io_context*)io_context;
	}
	void TCPClient::Connect(const char* ip, int port) {
		for (int i = 0; i < messages.size(); i++) {
			if (messages[i] == nullptr)continue;
			unsigned char* data = messages[i]->GetData();
			if (data == nullptr)continue;
			delete data;
		}
		messages.clear();
		is_disconnecting = false;
		is_writing = false;

		asio::io_context* context = (asio::io_context*)io_context;
		tcp::resolver resolver(*context);

		asio::ip::tcp::resolver::results_type _endpoints = resolver.resolve(ip, std::to_string(port));
		is_attempting_connect = true;
		is_connected = false;
		asio::async_connect(*((tcp::socket*)socket), _endpoints, [this](asio::error_code ec, asio::ip::tcp::endpoint ep) {
			OnConnect(ec);
		});
	}

	void TCPClient::Disconnect() {
		try {
			if ((is_attempting_connect || is_connected) && !is_disconnecting) {
				is_disconnecting = true;

				asio::error_code ec;

				((tcp::socket*)socket)->cancel(ec);
				if (is_connected) {
					((tcp::socket*)socket)->shutdown(((tcp::socket*)socket)->shutdown_both);
				}

				((tcp::socket*)socket)->close(ec);
				if (!is_connected) {
					if (on_connect_failed)on_connect_failed(ec.message());
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

	void TCPClient::Post(Packet* packet) {
		packet->WriteLength();
		messages.push_back(packet);

		if (is_writing == false)
			AsyncWrite();
	}

	//#ifdef NETWORKING_EXPORTS
	void TCPClient::AsyncRead() {
		//socket->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), std::bind(&TCPClient::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));

		((tcp::socket*)socket)->async_read_some(asio::buffer(this->read_buffer, this->read_buffer.size()), [this](asio::error_code ec, size_t bytes_transferred) {
			if (is_disconnecting)return;
			OnRead(ec, bytes_transferred);
		});
	}

	bool TCPClient::IsValidMessage() {
		if (messages.size() < 1) return false;

		if (messages[0] == nullptr)return false;

		if (messages[0]->GetData() == nullptr)return false;

		return true;
	}

	void TCPClient::AsyncWrite() {
		is_writing = true;
		while (!IsValidMessage() && messages.size() > 0) {
			messages.erase(messages.begin());
		}

		if (messages.size() < 1) {
			is_writing = false;
			return;
		}
		
		Packet* packet = messages[0];
		unsigned char* data = packet->GetData();
		asio::async_write(*((tcp::socket*)socket), asio::buffer(packet->GetData(), packet->GetUsedSize()), [this, data](const asio::error_code& error, std::size_t bytes_transferred) {
			OnWrite(error, bytes_transferred);
		});
	}
	void TCPClient::OnConnect(const std::error_code& e) {
		if (is_disconnecting)return;

		if (e) {
			Disconnect();
			return;
		}

		is_connected = true;
		if (on_connected)on_connected();

		AsyncRead();
	}
	void TCPClient::OnRead(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			Disconnect();
			return;
		}

		/*
		We will create a new packet and destroy it but not its contents because its contents is the read buffer
		*/
		Packet* packet = new Packet(NETWORKING_PACKET_SIZE, this->read_buffer.data());
		int length = packet->GetInt();
		if (on_packet_read)on_packet_read(packet, bytes_transferred);
		AsyncRead();
	}
	void TCPClient::OnWrite(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			Disconnect();
			return;
		}
		delete messages[0]->GetData();
		delete messages[0];
		messages.erase(messages.begin());

		if (messages.size() > 1) {
			AsyncWrite();
		}
		else {
			is_writing = false;
		}

	}
	//#endif
}