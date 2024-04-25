#pragma once

#include <string>

#include <Networking/framework.h>

#ifndef NETWORKING_PACKET_SIZE
#define NETWORKING_PACKET_SIZE 2048
#endif

namespace T1WD {
	class Packet {
	public:
		NETWORKING_API Packet(int packet_size = NETWORKING_PACKET_SIZE);
		NETWORKING_API Packet(int size, unsigned char* data);
		NETWORKING_API ~Packet();

		NETWORKING_API void DeleteData();
		NETWORKING_API void WriteLength();
		NETWORKING_API int GetUsedSize() { return bit_index / 8 + 1; }

		NETWORKING_API void WriteByte(unsigned char value);
		NETWORKING_API void WriteInt(int value);
		NETWORKING_API void WriteUInt(unsigned int value);
		NETWORKING_API void WriteBool(bool value);
		NETWORKING_API void WriteString(const char* value);

		NETWORKING_API unsigned char GetByte();
		NETWORKING_API int GetInt();
		NETWORKING_API unsigned int GetUInt();
		NETWORKING_API bool GetBool();
		NETWORKING_API std::string GetString();

		NETWORKING_API unsigned char* GetData() { return data; }

		unsigned char* data = nullptr;	//The actual data inside the packet
		unsigned int bit_index = 0;	// The index that you can start writing or reading data at
		unsigned int packet_size = 0;
	};
}