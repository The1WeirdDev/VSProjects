#pragma once

#include <vector>
#include <system_error>
#include <functional>
#include <array>
#include <map>
#include <mutex>

#include <Networking/framework.h>
#include <Networking/Packet.h>

namespace T1WD {
	extern "C" NETWORKING_API class TCPConnection {
	public:
		TCPConnection(void* socket);
		~TCPConnection();

		void Disconnect();

		void AsyncRead();
		void AsyncWrite();
		void Post(Packet* packet, bool delete_packet_data = true);

		void OnRead(const std::error_code& error, std::size_t bytes_transferred);
		void OnWrite(const std::error_code& error, std::size_t bytes_transferred);
		
		bool NoValidMessages();

		unsigned short id = 0;
	private:
		static std::mutex mutex;
		std::array<unsigned char, NETWORKING_MAX_PACKET_SIZE> read_buffer;
		std::vector<Message> messages;

		bool is_writing = false;
		void* socket = nullptr;
	};
	extern "C" NETWORKING_API class TCPServer {
	public:
		static NETWORKING_API void Start(int port, unsigned short max_players = 20);
		static NETWORKING_API void Stop();
		static NETWORKING_API void Tick();

		//Sends packet to certain client
		static NETWORKING_API void SendPacket(unsigned short id, Packet* packet, bool delete_packet_data = true);
		//Sends packet to all connected clients
		static NETWORKING_API void SendPacket(Packet* packet, bool delete_packet_data = true);

		static void OnDisconnect(TCPConnection* connection);

		static NETWORKING_API std::function<void()> on_server_started;
		static NETWORKING_API std::function<void(bool, const char*)> on_server_stopped;

		static NETWORKING_API std::function<void(std::error_code ec)> on_server_start_failed;
		static NETWORKING_API std::function<void(unsigned short)> on_client_connected;
		static NETWORKING_API std::function<void(const char*)> on_client_failed_connect;
		static NETWORKING_API std::function<void(unsigned short)> on_client_disconnected;

		static NETWORKING_API std::function<void(unsigned short, Packet*, size_t)> on_packet_read;
	private:
		static void* io_context;
		static void* acceptor;
		static void* socket;
		static void StartAccept();

		static void OnAccept(const std::error_code& e);
		static std::map<unsigned short, TCPConnection*> connections;

		static int port;
		static unsigned short max_players;
	};
}