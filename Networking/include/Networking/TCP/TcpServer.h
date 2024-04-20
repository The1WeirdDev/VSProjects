#pragma once

#include <Networking/framework.h>

#ifdef NETWORKING_EXPORTS
#include <asio.hpp>
#endif

extern "C" NETWORKING_API class TCPServer {
public:
	NETWORKING_API TCPServer();
	NETWORKING_API TCPServer(int port);

	NETWORKING_API void Start(int port);
private:
#ifdef NETWORKING_EXPORTS
	asio::io_service io_service;
#endif
	int port = 8888;
};