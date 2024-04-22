#pragma once

#ifdef NETWORKING_EXPORTS
#include <asio.hpp>
#endif

#include <Networking/framework.h>
#include <Networking/Packet.h>

extern "C" NETWORKING_API class TCPConnection {
public:
	TCPConnection() {}
};

extern "C" NETWORKING_API class TCPServer {
public:
	static NETWORKING_API void Start(int port);
	static NETWORKING_API void Tick();
private:
#ifdef NETWORKING_EXPORTS
	static asio::io_service io_service;
	static asio::ip::tcp::acceptor* acceptor;
	static asio::ip::tcp::socket* socket;


	static void StartAccept();
	static void OnAccept(const asio::error_code& e);
	static void OnWrite(const asio::error_code& error, std::size_t bytes_transferred);
#endif
	static int port;
};