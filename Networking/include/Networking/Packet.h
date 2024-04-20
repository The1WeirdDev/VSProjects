#pragma once

#include <Networking/framework.h>

extern "C" NETWORKING_API class Packet {
public:
	static int packet_size;
public:
	NETWORKING_API Packet();
	NETWORKING_API ~Packet();

	NETWORKING_API void WriteLength();

	NETWORKING_API void WriteByte(unsigned char value);
	NETWORKING_API void WriteInt(int value);
	NETWORKING_API void WriteUInt(unsigned int value);
	NETWORKING_API void WriteBool(bool value);

	NETWORKING_API unsigned char GetByte();
	NETWORKING_API int GetInt();
	NETWORKING_API unsigned int GetUInt();
	NETWORKING_API bool GetBool();

	NETWORKING_API unsigned char* GetData() { return data; }

	unsigned char* data = nullptr;	//The actual data inside the packet
	unsigned int bit_index = 0;	// The index that you can start writing or reading data at
};