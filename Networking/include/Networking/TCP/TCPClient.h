#pragma once

#include <functional>
#include <array>
#include <memory>
#include <system_error>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#include <Networking/framework.h>

#include <Networking/Packet.h>

namespace T1WD {
	extern "C" NETWORKING_API class TCPClient : public std::enable_shared_from_this<TCPClient> {
	public:
		TCPClient();
		~TCPClient();

		void NETWORKING_API Connect(const char* ip, int port, bool close_existing = false);
		void NETWORKING_API Disconnect();

		void NETWORKING_API Post(Packet* packet, bool delete_packet = true);

		//Run in separate thread
		void NETWORKING_API Run();

		//Can be run from any thread
		void NETWORKING_API Stop();

#pragma region CALLBACKS
		std::function<void(std::error_code) > on_connect_failed;
		std::function<void()> on_connected;
		std::function<void(std::error_code&)> on_disconnected;

		std::function<void(Packet*, size_t)> on_packet_read;
		std::function<void(Packet*, size_t)> on_packet_wrote;
#pragma endregion
		std::vector<Message> messages;

		bool is_writing = false;
		bool is_attempting_connect = false;
		bool is_connected = false;
		bool is_disconnecting = false;
		bool is_running = false;
		bool created_thread = false;
	private:
		std::mutex mutex;
		std::thread thread;
		std::array<unsigned char, NETWORKING_MAX_PACKET_SIZE> read_buffer;
		void Close(std::error_code ec = std::error_code(0, std::system_category()));
		void AsyncRead();
		void AsyncWrite();

		void FreeMessages();

		bool NoValidMessages();

		void OnConnect(const std::error_code& e);
		void OnRead(const std::error_code& error, std::size_t bytes_transferred);
		void OnWrite(const std::error_code& error, std::size_t bytes_transferred);

		//Every asio object is kept as an void* pointer and coverted in cpp filed
		//This is happening so we dont have to include asio in our projects
		void* io_context = nullptr;
		void* socket = nullptr;
	};
}