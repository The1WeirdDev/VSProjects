#pragma once

#include <functional>
#include <array>
#include <memory>
#include <system_error>
#include <string>
#include <vector>

#include <Networking/framework.h>

#include <Networking/Packet.h>

namespace T1WD {
	extern "C" NETWORKING_API class TCPClient : public std::enable_shared_from_this<TCPClient> {
	public:
		NETWORKING_API TCPClient();
		NETWORKING_API ~TCPClient();

		void NETWORKING_API Connect(const char* ip, int port);
		void NETWORKING_API Disconnect();

		void NETWORKING_API Post(Packet* packet);

		//Run in seperate thread
		void NETWORKING_API Run();
		void NETWORKING_API Stop();

#pragma region CALLBACKS
		std::function<void(std::string) > on_connect_failed;
		std::function<void()> on_connected;
		std::function<void()> on_disconnected;

		std::function<void(Packet*, size_t)> on_packet_read;
#pragma endregion
		std::vector<Packet*> messages;

		bool is_writing = false;
		bool is_attempting_connect = false;
		bool is_connected = false;
		bool is_disconnecting = false;
	private:
		std::array<unsigned char, NETWORKING_PACKET_SIZE> read_buffer;
		void AsyncRead();
		void AsyncWrite();

		bool IsValidMessage();

		void OnConnect(const std::error_code& e);
		void OnRead(const std::error_code& error, std::size_t bytes_transferred);
		void OnWrite(const std::error_code& error, std::size_t bytes_transferred);

		//Every asio object is kept as an void* pointer and coverted in cpp filed
		//This is happening so we dont have to include asio in our projects
		void* io_context = nullptr;
		void* socket = nullptr;
	};
}