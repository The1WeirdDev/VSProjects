#pragma once

#include <vector>

#include <Networking/framework.h>
#include <Networking/Packet.h>

#ifdef NETWORKING_EXPORTS
#include <asio.hpp>

extern "C" NETWORKING_API class TCPConnection {
public:
	TCPConnection(asio::ip::tcp::socket* socket) { this->socket = socket; }
	asio::ip::tcp::socket* socket;
};
#endif

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
	static std::vector<TCPConnection*> connections;
#endif
	static int port;
};