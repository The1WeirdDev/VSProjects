#include <iostream>
#include <string>

#include <Networking/TCP/TCPServer.h>
#include <Networking/Packet.h>

std::string array_to_binary(unsigned char* data, int length) {
	std::string string;

	for (int i = 0; i < length; i++) {
		for (int index = 0; index < 8; index++) {
			string += ((data[i] << index) & 0b1000000) == 0 ? "0" : "1";
		}
		string += " ";
	}

	return string;
}
const char* int_to_binary(int x)
{
	static char b[32];
	for (int i = 0; i < 32; i++) {
		b[i] = (((x << i) & (0b10000000000000000000000000000000)) > 0 ? '1' : '0');
	}
    return b;
}
int main(int argc, char** argv) {
	Packet p;
	TCPServer::Start(8888);

	while (true) {
		TCPServer::Tick();
	}
}