#pragma once

#include <functional>
#include <array>
#include <memory>
#include <system_error>

#include <Networking/framework.h>

#include <Networking/Packet.h>

extern "C" NETWORKING_API class TCPClient : public std::enable_shared_from_this<TCPClient> {
public:
	NETWORKING_API TCPClient();
	NETWORKING_API ~TCPClient();

	void NETWORKING_API Connect(const char* ip, int port);
	void NETWORKING_API Disconnect();

	void NETWORKING_API Run();
	void NETWORKING_API Stop();

	bool is_attempting_connect = false;
	bool is_connected = false;
	bool is_disconnecting = false;
private:
	std::array<unsigned char, NETWORKING_PACKET_SIZE> read_buffer;
	void AsyncRead();

	void OnConnect(std::shared_ptr<TCPClient> self, const std::error_code& e);
	void OnRead(std::shared_ptr<TCPClient> self, const std::error_code& error, std::size_t bytes_transferred);

	void* io_context = nullptr;
	void* socket = nullptr;
};