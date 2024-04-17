#pragma once

#include <Networking/framework.h>

extern "C" NETWORKING_API class TCPClient{
public:
	NETWORKING_API TCPClient();
	NETWORKING_API TCPClient(const char* ip, int port);

	void NETWORKING_API Connect(const char* ip, int port);
};