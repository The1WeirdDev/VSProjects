#include "TcpServer.h"
#include <iostream>

#include <Networking/Packet.h>

#include <asio.hpp>

using asio::ip::tcp;


namespace T1WD {
#pragma region TCPConnection
	std::mutex TCPConnection::mutex;

	TCPConnection::TCPConnection(void* socket) {
		this->socket = socket;
	}
	TCPConnection::~TCPConnection() {
		//This gets called after Disconnect()

	}
	void TCPConnection::Disconnect() {
		std::error_code ec;
		((tcp::socket*)socket)->close(ec);

		if (ec) {
			//Process Error
			//printf("Error closing socket\n");
		}
		try {
			delete ((tcp::socket*)socket);
		}
		catch (std::exception& e) {
			printf("Failed to delete socket. Reason :");
			std::cout << e.what() << std::endl;
		}
	}
	void TCPConnection::AsyncRead() {
		//socket->async_read_some(asio::buffer(this->read_buffer, NETWORKING_PACKET_SIZE), std::bind(&TCPClient::OnRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
		read_buffer.fill(0);
		((tcp::socket*)socket)->async_read_some(asio::buffer(this->read_buffer, this->read_buffer.size()), [this](asio::error_code ec, size_t bytes_transferred) {
			OnRead(ec, bytes_transferred);																																		
		});
	}
	void TCPConnection::Post(Packet* packet, bool delete_packet_data) {
		packet->WriteLength();

		std::lock_guard<std::mutex> guard(mutex);
		messages.push_back(Message(packet, delete_packet_data));
		if (is_writing == false)
			AsyncWrite();
	}
	bool TCPConnection::NoValidMessages() {
		while (true) {
			if (messages.size() < 1) {
				return false;
			}

			if (messages[0].packet == nullptr) {
				messages.erase(messages.begin());
				continue;
			}

			if (messages[0].packet->GetData() == nullptr) {
				messages.erase(messages.begin());
				continue;
			}

			return true;
		}
	}
	void TCPConnection::AsyncWrite() {
		is_writing = NoValidMessages();
		if (!is_writing)return;

		Packet* packet = messages[0].packet;
		bool delete_data = messages[0].delete_packet;

		asio::async_write(*((tcp::socket*)socket), asio::buffer(packet->GetData(), packet->GetUsedSize()), [this, data = packet, delete_data](const asio::error_code& error, std::size_t bytes_transferred) {
			if (delete_data) {
				delete data;
			}
			OnWrite(error, bytes_transferred);
		});
	}
	void TCPConnection::OnRead(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			TCPServer::OnDisconnect(this);
			//TODO: Disconnect client
			return;
		}
		/*
		We will create a new packet and destroy it but not its contents because its contents is the read buffer
		*/
		//Packet* packet = new Packet(read_buffer.size(), read_buffer.data());
		//int length = packet->GetInt();
		//if (TCPServer::on_packet_read)TCPServer::on_packet_read(id, packet, bytes_transferred);

		//AsyncRead();
		unsigned int offset = 0;

		while (true) {
			if (bytes_transferred < 4 || bytes_transferred > NETWORKING_MAX_PACKET_SIZE) {
				AsyncRead();
				break;
			}

			Packet packet(bytes_transferred, this->read_buffer.data() + offset);
			int length = packet.GetInt() + 1;
			if (length < 4 || length > NETWORKING_MAX_PACKET_SIZE) {
				AsyncRead();
				return;
			}
			if (TCPServer::on_packet_read)TCPServer::on_packet_read(id, &packet, bytes_transferred);

			bytes_transferred -= length;
		}
	}

	void TCPConnection::OnWrite(const std::error_code& error, std::size_t bytes_transferred) {
		if (error) {
			printf("Error writing to tcp clients.\n");
			TCPServer::OnDisconnect(this);
			return;
		}
		else {
			printf("Transferred %d bytes.\n", (int)bytes_transferred);
		}
		messages.erase(messages.begin());
		AsyncWrite();
	}

#pragma endregion
	int TCPServer::port = 8888;
	unsigned short TCPServer::max_players = 20;
	std::map<unsigned short, TCPConnection*> TCPServer::connections;
	void* TCPServer::io_context;
	void* TCPServer::acceptor;
	void* TCPServer::socket;

	std::function<void(unsigned short)> TCPServer::on_client_connected = nullptr;
	std::function<void(bool, const char*)> TCPServer::on_server_stopped = nullptr;

	std::function<void(const char*) > TCPServer::on_client_failed_connect;
	std::function<void(unsigned short)> TCPServer::on_client_disconnected = nullptr;
	std::function<void()> TCPServer::on_server_started = nullptr;
	std::function<void(std::error_code)> TCPServer::on_server_start_failed = nullptr;

	std::function<void(unsigned short, Packet*, size_t)> TCPServer::on_packet_read = nullptr;

	void TCPServer::Start(int port, unsigned short max_players) {
		TCPServer::port = port;
		TCPServer::max_players = max_players;
		connections.clear();

		io_context = new asio::io_context();
		acceptor = new tcp::acceptor(*(asio::io_context*)io_context, tcp::endpoint(tcp::v4(), port));

		if (on_server_started)on_server_started();
		StartAccept();
	}

	void TCPServer::Stop() {
		for (auto i : connections) {
			i.second->Disconnect();
		}

		((tcp::acceptor*)acceptor)->close();
		delete ((tcp::acceptor*)acceptor);

		delete ((asio::io_context*)io_context);
	}

	void TCPServer::Tick() {
		((asio::io_context*)io_context)->poll();
	}

	void TCPServer::SendPacket(unsigned short id, Packet* packet, bool delete_packet_data) {
		std::map<unsigned short, TCPConnection*>::iterator connection = connections.find(id);
		if (connection != connections.end()) {
			connection->second->Post(packet, delete_packet_data);
		}
	}

	void TCPServer::SendPacket(Packet* packet, bool delete_packet_data) {
		std::map<unsigned short, TCPConnection*>::iterator connection;
		for (connection = connections.begin(); connection != connections.end(); connection++) {

			connection->second->Post(packet, delete_packet_data);
		}
	}
	void TCPServer::StartAccept() {
		socket = new tcp::socket(*(asio::io_context*)io_context);
		((tcp::acceptor*)acceptor)->async_accept(*((tcp::socket*)socket), &TCPServer::OnAccept);
	}

	void TCPServer::OnAccept(const std::error_code& e) {
		if (e) {
			printf("Error accepting clients. ");
			std::cout << e.message() << std::endl;
			if (on_server_stopped)on_server_stopped(true, e.message().c_str());
			return;
		}
		TCPConnection* connection = new TCPConnection(socket);
		for (int i = 0; i < max_players; i++) {
			if (connections.find(i) == connections.end()) {
				connections.emplace(i, connection);
				connection->id = i;

				connection->AsyncRead();
				if (on_client_connected)on_client_connected(i);
				StartAccept();
				return;
			}
		}
		if (TCPServer::on_client_failed_connect)on_client_failed_connect("Server Full");
		connection->Disconnect();
		StartAccept();
		return;
	}
	void TCPServer::OnDisconnect(TCPConnection* connection) {
		if (connections.find(connection->id) != connections.end()) {
			connections.erase(connection->id);
		}
		connection->Disconnect();
		if (TCPServer::on_client_disconnected)TCPServer::on_client_disconnected(connection->id);
		delete connection;
	}
}