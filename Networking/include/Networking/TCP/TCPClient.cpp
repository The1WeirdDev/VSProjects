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
		socket = nullptr;
	}
	TCPClient::~TCPClient() {
		delete ((asio::io_context*)io_context);
	}
	void TCPClient::Connect(const char* ip, int port) {
		if (socket != nullptr) {
			Close();
		}
		socket = (void*)new tcp::socket(*(asio::io_context*)io_context);

		tcp::resolver resolver(*(asio::io_context*)io_context);
		asio::ip::tcp::resolver::results_type _endpoints = resolver.resolve(ip, std::to_string(port));

		is_disconnecting = false;
		is_writing = false;
		is_attempting_connect = true;
		is_connected = false;

		tcp::socket* sok = (tcp::socket*)socket;
		asio::async_connect(*((tcp::socket*)socket), _endpoints, [this, sok](asio::error_code ec, asio::ip::tcp::endpoint ep) {
			if (sok)OnConnect(ec);
		});

		Run();
	}

	void TCPClient::Disconnect() {
		try {
			Close();
		}
		catch (std::exception& e) {
			printf("Exception closing socket %s\n", e.what());
		}
	}

	void TCPClient::Close(std::error_code error) {
		try {
			if ((is_connected || is_attempting_connect) && !is_disconnecting) {
				is_disconnecting = true;

				asio::error_code ec;

				((tcp::socket*)socket)->cancel(ec);

				if (is_connected) {
					((tcp::socket*)socket)->shutdown(((tcp::socket*)socket)->shutdown_both, ec);
					if (on_disconnected)on_disconnected(error);
				}
				else
					if (on_connect_failed)on_connect_failed(error);

				if(((tcp::socket*)socket)->is_open())
					((tcp::socket*)socket)->close(ec);

				FreeMessages();
				is_attempting_connect = false;
				is_connected = false;
				is_writing = false;
				is_disconnecting = false;

				delete (tcp::socket*)socket;
				socket = nullptr;
			}
		}
		catch (std::exception& e) {
			printf("Exception closing socket %s\n", e.what());
		}
	}


	void TCPClient::OnConnect(const std::error_code& e) {
		if (e) {
			Close();
			return;
		}

		is_attempting_connect = false;
		is_connected = true;
		if (on_connected)on_connected();
		AsyncRead();
	}

	void TCPClient::Run() {
		if (is_running)return;
		is_running = true;

		try {
			if (created_thread == false) {
				thread = std::thread([this]() {
					while (is_running) {
						if (is_attempting_connect || is_connected || is_disconnecting) {
							((asio::io_context*)io_context)->poll_one();
						}
					}
				});

				created_thread = true;
			}
			
		}
		catch (std::exception& e) {
			printf("Error with io context %s\n", e.what());
			//delete ((asio::io_context*)io_context);
			is_running = false;
		}
	}

	void TCPClient::Stop() {
		if (is_running == false)return;
		is_running = false;
		if (is_connected || is_attempting_connect)
			Close();
		((asio::io_context*)io_context)->stop();
		thread.join();
	}

	void TCPClient::FreeMessages() {
		for (int i = 0; i < messages.size(); i++) {
			if (messages[i] == nullptr)continue;
			unsigned char* data = messages[i]->GetData();
			if (data == nullptr)continue;
			delete data;
		}
	}

	void TCPClient::Post(Packet* packet) {
		packet->WriteLength();
		messages.push_back(packet);

		if (is_writing == false)
			AsyncWrite();
	}

	//#ifdef NETWORKING_EXPORTS
	void TCPClient::AsyncRead() {

		((tcp::socket*)socket)->async_read_some(asio::buffer(this->read_buffer.data(), this->read_buffer.size()), [this](asio::error_code ec, size_t bytes_transferred) {
			if (!is_disconnecting && is_connected) {
				OnRead(ec, bytes_transferred);
			}
		});
		
	}

	void TCPClient::OnRead(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			Close(error);
			return;
		}
		//We will create a new packet and destroy it but not its contents because its contents is the read buffer

		Packet* packet = new Packet(this->read_buffer.size(), this->read_buffer.data());
		int length = packet->GetInt();
		if (on_packet_read)on_packet_read(packet, bytes_transferred);
		AsyncRead();
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
			if (!is_disconnecting && is_connected) {
				OnWrite(error, bytes_transferred);
			}
		});
	}
	void TCPClient::OnWrite(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			Close(error);
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