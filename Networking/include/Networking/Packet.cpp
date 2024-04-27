#include "Packet.h"

#include <array>
#include <iostream>
#include <math.h>
#include <stdexcept>

namespace T1WD {
	Packet::Packet(int packet_size) {
		this->packet_size = packet_size;
		data = new unsigned char[packet_size];
		std::fill(data, data + packet_size, 0);
		bit_index = 32;
	}
	Packet::Packet(int size, unsigned char* data) {
		packet_size = size;
		this->data = data;
		bit_index = 0;	//0 so we can read the length of the buffer
	}
	Packet::~Packet() {

	}

	void Packet::DeleteData() {
		delete data;
		data = nullptr;
		packet_size = 0;
	}

	void Packet::WriteLength() {
		//The first 4 bytes are reserved just for packet length
		int bit = bit_index;
		bit_index = 0;

		WriteInt(bit / 8);	//Write Size in bytes

		bit_index = bit;
	}

	void Packet::WriteByte(unsigned char value) {
		int byte_pos = bit_index / 8;
		unsigned char filled_bits = bit_index % 8;
		unsigned char remaining = 8 - filled_bits;


		data[byte_pos] |= value >> filled_bits;
		data[byte_pos + 1] |= (value & ((1 << remaining) - 1)) << remaining;

		bit_index += 8;
	}
	void Packet::WriteInt(int value) {
		int byte_pos = bit_index / 8;
		unsigned char filled_bits = bit_index % 8;
		unsigned char remaining = 8 - filled_bits;

		data[byte_pos] |= (value >> 24) >> filled_bits;
		data[byte_pos + 1] |= (value >> 16) >> filled_bits;
		data[byte_pos + 2] |= (value >> 8) >> filled_bits;
		data[byte_pos + 3] |= value >> filled_bits;
		data[byte_pos + 4] |= (value & ((1 << remaining) - 1)) << remaining;

		bit_index += 32;
	}

	void Packet::WriteUInt(unsigned int value) {
		int byte_pos = bit_index / 8;
		unsigned char filled_bits = bit_index % 8;
		unsigned char remaining = 8 - filled_bits;

		data[byte_pos] |= (value >> 24) >> filled_bits;
		data[byte_pos + 1] |= (value >> 16) >> filled_bits;
		data[byte_pos + 2] |= (value >> 8) >> filled_bits;
		data[byte_pos + 3] |= value >> filled_bits;
		data[byte_pos + 4] |= (value & ((1 << remaining) - 1)) << remaining;

		bit_index += 32;
	}

	void Packet::WriteBool(bool value) {
		unsigned char index = bit_index % 8;
		data[bit_index / 8] |= (value << (7 - index));
		bit_index++;
	}

	void Packet::WriteString(const char* value) {
		for (int i = 0; i < strlen(value); i++) {
			WriteByte((unsigned char)value[i]);
		}
		WriteByte(0x00);
	}
	void Packet::WriteString(std::string& value) {
		for (int i = 0; i < value.size(); i++) {
			WriteByte((unsigned char)value[i]);
		}
		WriteByte(0x00);
	}
	void Packet::WriteString(std::string value) {
		for (int i = 0; i < value.size(); i++) {
			WriteByte((unsigned char)value[i]);
		}
		WriteByte(0x00);
	}

	unsigned char Packet::GetByte() {
#ifdef NETWORKING_READ_SAFE
		if (byte_pos / 8 + 2 <= packet_size) {
			throw std::range_error("Unable to read byte. Not enough bytes left to read.\n");
			return 0;
		}
#endif
		//A byte is 1 byte/8 bits
		int byte_pos = bit_index / 8;
		unsigned char filled_bits = bit_index % 8;
		unsigned char remaining = 8 - filled_bits;

		int value = data[byte_pos] << filled_bits;
		value |= (data[byte_pos + 1] >> (8 - filled_bits));
		bit_index += 8;
		return value;
	}
	int Packet::GetInt() {
#ifdef NETWORKING_READ_SAFE
		if (byte_pos / 8 + 5 <= packet_size) {
			throw std::range_error("Unable to read int. Not enough bytes left to read.\n");
			return 0;
		}
#endif
		//An integer is 4 bytes
		int byte_pos = bit_index / 8;
		unsigned char filled_bits = bit_index % 8;
		unsigned char remaining = 8 - filled_bits;

		int value = (data[byte_pos]) << 24 << filled_bits;
		value |= (data[byte_pos + 1] << 16) << filled_bits;
		value |= (data[byte_pos + 2] << 8) << filled_bits;
		value |= (data[byte_pos + 3]) << filled_bits;
		value |= (data[byte_pos + 4] >> (8 - filled_bits));
		bit_index += 32;
		return value;
	}

	unsigned int Packet::GetUInt() {
#ifdef NETWORKING_READ_SAFE
		if (byte_pos / 8 + 5 <= packet_size) {
			throw std::range_error("Unable to read unsigned int. Not enough bytes left to read.\n");
			return 0;
		}
#endif
		//An integer is 4 bytes
		unsigned char remaining = bit_index % 8;
		unsigned char filled_bits = 8 - remaining;
		int byte_pos = bit_index / 8;

		unsigned int value = (data[byte_pos] & ((1 << (remaining)) - 1)) << 24;
		value |= (data[byte_pos + 1] << 16) << filled_bits;
		value |= (data[byte_pos + 2] << 8) << filled_bits;
		value |= (data[byte_pos + 3] << filled_bits);
		value |= (data[byte_pos + 4] >> filled_bits);
		bit_index += 32;
		return value;
	}


	bool Packet::GetBool() {
#ifdef NETWORKING_READ_SAFE
		if (byte_pos / 8 + 1 <= packet_size) {
			throw std::range_error("Unable to read bool. Not enough bytes left to read.\n");
			return 0;
		}
#endif
		unsigned char remaining = bit_index % 8;
		bool value = (1 & (data[bit_index / 8] >> (7 - bit_index))) > 0 ? true : false;
		bit_index++;
		return value;
	}

	std::string Packet::GetString() {
		//TODO: Catch errors
		std::string string;
		unsigned char byte = 0x00;
		while (true) {
			byte = GetByte();
			if (byte == 0x00 || bit_index / 8 >= packet_size)break;
			string = string + (char)byte;
		}

		return string;
	}
}