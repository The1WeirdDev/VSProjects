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
	printf("TEST1\n");
	p.bit_index = 0;
	p.WriteBool(true);
	p.WriteBool(true);
	p.WriteBool(true);
	p.WriteBool(true);
	p.WriteBool(true);
	p.WriteBool(true);
	p.WriteBool(true);
	p.WriteInt(513);
	//p.WriteInt(232165);
	//p.WriteUInt(UINT_MAX);
	p.WriteLength();
	p.bit_index -= 32;
	unsigned char* data = p.GetData();
	std::cout << p.GetInt() << std::endl;
	printf("%s\n", array_to_binary(data, 5).c_str());
	/*
	p.WriteBool(true);
	p.WriteBool(false);
	p.WriteBool(true);
	p.WriteInt(256);
	p.WriteInt(512);
	p.WriteBool(true);
	p.WriteBool(true);
	p.WriteBool(false);
	p.WriteBool(false);
	p.WriteBool(true);
	printf("TEST2\n");
	p.bit_index = 0;
	bool b1 = p.GetBool();
	bool b2 = p.GetBool();
	bool b3 = p.GetBool();
	int int1 = p.GetInt();
	int int2 = p.GetInt();
	bool b4 = p.GetBool();
	bool b5 = p.GetBool();
	bool b6 = p.GetBool();
	bool b7 = p.GetBool();
	bool b8 = p.GetBool();
	printf("INT %i %i %i %d %d %i %i %i %i %i\n", b1, b2, b3, int1, int2, b4, b5, b6, b7, b8);\*/
	TCPServer server(8888);
}