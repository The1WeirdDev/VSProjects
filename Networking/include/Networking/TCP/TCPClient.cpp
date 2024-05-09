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
			if (messages[i].packet == nullptr)continue;
			unsigned char* data = messages[i].packet->GetData();
			if (data == nullptr)continue;
			delete data;
			delete messages[i].packet;
		}
	}

	//#ifdef NETWORKING_EXPORTS
	void TCPClient::AsyncRead() {
		read_buffer.fill(0);
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

		if (bytes_transferred < 4) {
			AsyncRead();
			return;
		}

		//Packet packet(bytes_transferred, this->read_buffer.data());
		//int length = packet.GetInt() + 1;
		unsigned int offset = 0;

		while (true) {
			if (bytes_transferred < 4 || bytes_transferred > NETWORKING_MAX_PACKET_SIZE) {
				AsyncRead();
				break;
			}

			Packet packet(bytes_transferred, this->read_buffer.data() + offset);
			int length = packet.GetInt() + 1;
			if (length < 4 || length > NETWORKING_MAX_PACKET_SIZE) {
				AsyncRead();
				return;
			}
			if (on_packet_read)on_packet_read(&packet, bytes_transferred);

			bytes_transferred -= length;
		}

		//read_buffer.fill(0);

		//AsyncRead();
	}
	void TCPClient::Post(Packet* packet, bool delete_packet_data) {
		packet->WriteLength();

		std::lock_guard<std::mutex> guard(mutex);
		messages.push_back(Message(packet, delete_packet_data));
		if (is_writing == false)
			AsyncWrite();
	}
	bool TCPClient::NoValidMessages() {
		while (true) {
			if (messages.size() < 1) {
				return false;
			}

			if (messages[0].packet == nullptr) {
				messages.erase(messages.begin());
				continue;
			}

			if (messages[0].packet->GetData() == nullptr) {
				messages.erase(messages.begin());
				continue;
			}

			return true;
		}
	}
	void TCPClient::AsyncWrite() {
		is_writing = NoValidMessages();
		if (!is_writing)return;

		Packet* packet = messages[0].packet;
		bool delete_data = messages[0].delete_packet;

		asio::async_write(*((tcp::socket*)socket), asio::buffer(packet->GetData(), packet->GetUsedSize()), [this, data = packet, delete_data](const asio::error_code& error, std::size_t bytes_transferred) {
			if (delete_data) {
				delete data;
			}
			OnWrite(error, bytes_transferred);
		});
	}

	void TCPClient::OnWrite(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			Close(error);
			return;
		}
		else {
			if (on_packet_wrote)on_packet_wrote(messages[0].packet, bytes_transferred);
		}
		messages.erase(messages.begin());
		AsyncWrite();
	}
	//#endif
}