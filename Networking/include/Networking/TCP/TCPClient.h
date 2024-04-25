#pragma once

#include <functional>
#include <array>
#include <memory>
#include <system_error>
#include <string>

#include <Networking/framework.h>

#include <Networking/Packet.h>

namespace T1WD {
	extern "C" NETWORKING_API class TCPClient : public std::enable_shared_from_this<TCPClient> {
	public:
		NETWORKING_API TCPClient();
		NETWORKING_API ~TCPClient();

		void NETWORKING_API Connect(const char* ip, int port);
		void NETWORKING_API Disconnect();

		//Run in seperate thread
		void NETWORKING_API Run();
		void NETWORKING_API Stop();

		std::function<void(std::string) > on_connect_failed;
		std::function<void()> on_connected;
		std::function<void()> on_disconnected;

		std::function<void(Packet*, size_t bytes_transferred)> on_packet_read;

		bool is_attempting_connect = false;
		bool is_connected = false;
		bool is_disconnecting = false;
	private:
		std::array<unsigned char, NETWORKING_PACKET_SIZE> read_buffer;
		void AsyncRead();
		void AsyncWrite();

		void OnConnect(std::shared_ptr<TCPClient> self, const std::error_code& e);
		void OnRead(const std::error_code& error, std::size_t bytes_transferred);
		void OnWrite(const std::error_code& error, std::size_t bytes_transferred);

		void* io_context = nullptr;
		void* socket = nullptr;
	};
}