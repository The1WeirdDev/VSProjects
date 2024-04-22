#pragma once

#include <functional>
#include <array>

#include <Networking/framework.h>

#ifdef NETWORKING_EXPORTS
#include <asio.hpp>
#endif

#include <Networking/Packet.h>

extern "C" NETWORKING_API class TCPClient{
public:
	NETWORKING_API TCPClient();
	NETWORKING_API ~TCPClient();

	void NETWORKING_API Connect(const char* ip, int port);
	void NETWORKING_API Disconnect();;
private:
	std::array<unsigned char, NETWORKING_PACKET_SIZE> read_buffer;
#ifdef NETWORKING_EXPORTS

	void AsyncRead();

	void OnConnect(const asio::error_code& e);
	void OnRead(const asio::error_code& error, std::size_t bytes_transferred);

	asio::io_context io_context;
	asio::ip::tcp::socket socket;
	asio::ip::tcp::resolver::results_type _endpoints;

#endif
};