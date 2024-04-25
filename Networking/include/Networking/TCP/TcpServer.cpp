#include "TcpServer.h"
#include <iostream>

#include <Networking/Packet.h>

#include <asio.hpp>

using asio::ip::tcp;

namespace T1WD {
	class TCPConnection {
	public:
		TCPConnection(tcp::socket* socket) { this->socket = socket; }

		void Post(Packet* packet) {

		}

		tcp::socket* socket;
	};

	int TCPServer::port = 8888;
	std::vector<TCPConnection*> TCPServer::connections;

	void* TCPServer::io_context;
	void* TCPServer::acceptor;
	void* TCPServer::socket;

	std::function<void()> TCPServer::on_client_connected = nullptr;
	std::function<void()> TCPServer::on_server_started = nullptr; 
	std::function<void(std::error_code)> TCPServer::on_server_start_failed = nullptr;

	void TCPServer::Start(int port) {
		TCPServer::port = port;
		connections.clear();

		io_context = new asio::io_context();
		acceptor = new tcp::acceptor(*(asio::io_context*)io_context);
		((tcp::acceptor*)acceptor)->open(tcp::v4());

		asio::error_code ec;
		((tcp::acceptor*)acceptor)->bind(tcp::endpoint(tcp::v4(), port), ec);

		if (ec) {
			if (on_server_start_failed)on_server_start_failed(ec);
		}
		else {
			if (on_server_started)on_server_started();
			StartAccept();
			((asio::io_context*)io_context)->poll();
		}
	}

	void TCPServer::Tick() {
		((asio::io_context*)io_context)->poll();
	}
	void TCPServer::StartAccept() {
		socket = new tcp::socket(*(asio::io_context*)io_context);
		((tcp::acceptor*)acceptor)->async_accept(*((tcp::socket*)socket), &TCPServer::OnAccept);
	}

	void TCPServer::OnAccept(const std::error_code& e) {
		std::string message = "Hi\n";
		asio::error_code ignored_error;

		TCPConnection* connection = new TCPConnection((tcp::socket*)socket);
		connections.push_back(connection);

		if(on_client_connected)on_client_connected();
		//unsigned char* data = packet.GetData();
		//asio::async_write(*((tcp::socket*)socket), asio::buffer(packet.GetData(), packet.GetUsedSize()), [data](const asio::error_code& error, std::size_t bytes_transferred) {
		//	delete data;
		//	OnWrite(error, bytes_transferred);
		//});
		StartAccept();
	}

	void TCPServer::OnWrite(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			printf("Error writing to tcp clients.\n");
		}
		else {
			printf("Transferred %d bytes.\n", (int)bytes_transferred);
		}
	}
}