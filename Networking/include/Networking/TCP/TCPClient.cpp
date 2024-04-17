#include "TCPClient.h"
#include <iostream>


TCPClient::TCPClient() {}
TCPClient::TCPClient(const char* ip, int port) {
	Connect(ip, port);
}

void TCPClient::Connect(const char* ip, int port) {
	printf("HELLO NETORKING\n");
}