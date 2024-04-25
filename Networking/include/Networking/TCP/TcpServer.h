#pragma once

#include <vector>
#include <system_error>
#include <functional>

#include <Networking/framework.h>
#include <Networking/Packet.h>

namespace T1WD {

	extern "C" NETWORKING_API class TCPConnection;
	extern "C" NETWORKING_API class TCPServer {
	public:
		static NETWORKING_API void Start(int port);
		static NETWORKING_API void Tick();

		static NETWORKING_API std::function<void()> on_server_started;
		static NETWORKING_API std::function<void(std::error_code ec)> on_server_start_failed;
		static NETWORKING_API std::function<void()> on_client_connected;
	private:
		//static asio::io_service io_service;
		//static asio::ip::tcp::acceptor* acceptor;
		//static asio::ip::tcp::socket* socket;
		static void* io_context;
		static void* acceptor;
		static void* socket;
		static void StartAccept();

		static void OnAccept(const std::error_code& e);
		static void OnWrite(const std::error_code& error, std::size_t bytes_transferred);
		static std::vector<TCPConnection*> connections;

		static int port;
	};
}