#pragma once

#include <string>
#include <array>

#include <Networking/framework.h>

/*
NETWORKING_PACKET_SIZE is by default 2048 bytes
This is a arbitrary number no reason for it being picked
Should Normally read 2048 bytes but for writing just write GetUsedSize() bytes;
*/
#ifndef NETWORKING_MAX_PACKET_SIZE
#define NETWORKING_MAX_PACKET_SIZE 2048
#endif

//NETWORKING_READ_SAFE
/*
if NETWORKING_READ_SAFE is defined then geting valued will be safer
this should be used on the server incase client sends invalid packet(hacket maybe)
*/

namespace T1WD {
	class Packet {
	public:
		//This is just a default value
		NETWORKING_API Packet(int packet_size = NETWORKING_MAX_PACKET_SIZE);
		NETWORKING_API Packet(int size, unsigned char* data);
		NETWORKING_API ~Packet();

		NETWORKING_API void DeleteData();
		NETWORKING_API void WriteLength();
		NETWORKING_API inline int GetUsedSize() { return bit_index / 8 + 1; }

		//Maybe we can save some size in the library by having these writing functions-
		//Maybe write only 1 2 3 or 4 bytes.
		//TODO: Add a check when writing to see if we have available space
		NETWORKING_API void WriteChar(char value);
		NETWORKING_API void WriteUChar(unsigned char value);
		NETWORKING_API void WriteShort(short value);
		NETWORKING_API void WriteUShort(unsigned short value);
		NETWORKING_API void WriteInt(int value);	//Will always be 4 bytes
		NETWORKING_API void WriteUInt(unsigned int value);
		NETWORKING_API void WriteFloat(float value);
		NETWORKING_API void WriteBool(bool value);
		NETWORKING_API void WriteString(const char* value);
		NETWORKING_API void WriteString(std::string& value);
		NETWORKING_API void WriteString(std::string value);

		NETWORKING_API void WriteUCharArray(unsigned int array_size, unsigned char* array);
		NETWORKING_API void WriteCharArray(unsigned int array_size, char* array);

		NETWORKING_API char GetChar();
		NETWORKING_API unsigned char GetUChar();
		NETWORKING_API short GetShort();
		NETWORKING_API unsigned short GetUShort();
		NETWORKING_API int GetInt();
		NETWORKING_API unsigned int GetUInt();
		NETWORKING_API float GetFloat();
		NETWORKING_API bool GetBool();
		NETWORKING_API std::string GetString();
		NETWORKING_API unsigned char* GetUCharArray(size_t* length);
		NETWORKING_API char* GetCharArray(size_t* length);

		//NETWORKING_API unsigned char* GetData() { return data; }
		unsigned char* GetData() { return data; }
		unsigned char* data = nullptr;	//The actual data inside the packet
		unsigned int bit_index = 0;	// The index that you can start writing or reading data at
		unsigned int packet_size = 0; // The size when reading for data
		bool delete_data = false; //On deconstructed should we delete packet data aswell?
	};

	struct Message {
		Packet* packet = nullptr;
		bool delete_packet = true;
		unsigned short amount_sent = 0;

		Message(Packet* packet, bool delete_packet = true) {
			this->packet = packet;
			this->delete_packet = delete_packet;
		}
	};
}