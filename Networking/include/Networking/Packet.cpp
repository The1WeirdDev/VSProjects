#include "Packet.h"
#include <array>
#include <iostream>
int Packet::packet_size = 1024;
Packet::Packet(){
	data = new unsigned char[packet_size];
	std::fill(data, data + packet_size, 0);
}
Packet::~Packet(){}

void Packet::WriteLength() {
	//The first 4 bytes are reserved just for packet length
}

void Packet::WriteByte(unsigned char value) {
	int byte_pos = bit_index / 8;
	unsigned char filled_bits = 8 - (bit_index % 8);
	data[byte_pos] |= value >> filled_bits;
	data[byte_pos + 1] |= value << filled_bits;
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
	data[byte_pos + 4] |= (value & ((1 << remaining) - 1) << (8 - remaining));
	bit_index += 32;
}

void Packet::WriteUInt(unsigned int value) {
	int byte_pos = bit_index / 8;
	unsigned char filled_bits = 8 - (bit_index % 8);

	data[byte_pos] |= (value >> 24) >> filled_bits;
	data[byte_pos + 1] |= (value >> 16) >> filled_bits;
	data[byte_pos + 2] |= (value >> 8) >> filled_bits;
	data[byte_pos + 3] |= value >> filled_bits;
	data[byte_pos + 4] |= value << filled_bits;
	bit_index += 32;
}

void Packet::WriteBool(bool value) {
	unsigned char index = bit_index % 8;
	printf("Index %d %d\n", index, (value << (7 - index)));
	data[bit_index / 8] |= (value << (7 - index));
	bit_index++;
}

unsigned char Packet::GetByte() {
	//An integer is 4 bytes
	unsigned char filled_bits = 8 - (bit_index % 8);
	int byte_pos = bit_index / 8;

	int value = (data[byte_pos] << filled_bits);
	value |= (data[byte_pos + 1] >> filled_bits);
	bit_index += 8;
	return value;
}
int Packet::GetInt() {
	//An integer is 4 bytes
	int byte_pos = bit_index / 8;
	unsigned char filled_bits = bit_index % 8;
	unsigned char remaining = 8 - filled_bits;

	int value = (data[byte_pos] & ((1 << (remaining)) - 1)) << 24;
	value |= (data[byte_pos + 1] << 16) << filled_bits;
	value |= (data[byte_pos + 2] << 8) << filled_bits;
	value |= (data[byte_pos + 3] << filled_bits);
	value |= (data[byte_pos + 4] >> (8 - remaining));
	bit_index += 32;
	return value;
}

unsigned int Packet::GetUInt() {
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
	unsigned char remaining = bit_index % 8;
	bool value = (1 & (data[bit_index / 8] >> (7 - bit_index))) > 0 ? true : false;
	bit_index++;
	return value;
}