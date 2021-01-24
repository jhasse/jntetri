#include "Client.hpp"

#include "Server.hpp"

#include <boost/array.hpp>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

Client::Client(Server& server) : socket(context), server(server) {
}

tcp::socket& Client::getSocket() {
	return socket;
}

const char DELIMITER = '\b';

std::stringstream receive(tcp::socket& socket) {
	std::stringstream sstream;
	while (true) {
		boost::array<char, 128> buf;
		const size_t len = socket.read_some(boost::asio::buffer(buf));
		std::cout << "Received " << len << " bytes." << std::endl;
		for (size_t i = 0; i < len; ++i) {
			if (buf[i] == DELIMITER) {
				return sstream;
			}
			sstream << buf[i];
		}
	}
}

void asyncReceive(tcp::socket& socket, std::function<void(std::stringstream)> handler) {
	struct ReadHandler {
		tcp::socket& socket;
		std::unique_ptr<std::array<char, 128>> buf;
		std::unique_ptr<std::stringstream> sstream;
		std::function<void(std::stringstream)> handler;

		ReadHandler(const ReadHandler&) = delete;
		ReadHandler& operator=(const ReadHandler&) = delete;
		ReadHandler(ReadHandler&&) = default;
		~ReadHandler() {
			if (sstream) {
				handler(std::move(*sstream));
			}
		}

		void operator()(const boost::system::error_code& err, size_t len) {
			if (err) {
				throw std::runtime_error("Receive Error");
			}
			std::cout << "Received " << len << " bytes." << std::endl;
			for (size_t i = 0; i < len; ++i) {
				if ((*buf)[i] == DELIMITER) {
					return;
				}
				*sstream << (*buf)[i];
			}
			socket.async_receive(boost::asio::buffer(*buf), std::move(*this));
		}
	};

	ReadHandler readHandler{ socket, std::make_unique<std::array<char, 128>>(),
		                     std::make_unique<std::stringstream>(), std::move(handler) };

	auto mutableBuf = boost::asio::buffer(*readHandler.buf);
	socket.async_receive(mutableBuf, std::move(readHandler));
}

void Client::run() {
	uint32_t protocolVersion;
	receive(socket) >> protocolVersion;
	std::cout << "Protocol version: " << protocolVersion << std::endl;
	if (protocolVersion != 1) {
		return;
	}
	socket.send(boost::asio::buffer({ 'o', 'k', '\b' }));
	std::cout << "Sent \"ok\\b\"." << std::endl;
	auto sstream = receive(socket);
	std::string command;
	sstream >> command;
	if (command == "login") {
		std::string user;
		std::string password;
		sstream >> user >> password;
		std::cout << "Logging in '" << user << "' with password of length " << password.size()
		          << "." << std::endl;
		if (password == "asd") {
			std::cout << "Accepted password, sending \"ok\\b\" ..." << std::endl;
			socket.send(boost::asio::buffer({ 'o', 'k', '\b' }));
			username = user;
			asyncReceive(socket,
			             [this](std::stringstream sstream) { handleRecv(std::move(sstream)); });
			context.run();
		} else {
			socket.send(boost::asio::buffer("wrong password\b"));
		}
	} else {
		throw std::runtime_error("Unknown command '" + command + "'.");
	}
}

void Client::setOpponent(std::shared_ptr<Client> opponent) {
	this->opponent = std::move(opponent);
}

std::string Client::getUsername() const {
	return username;
}

void Client::forward(uint8_t time, uint8_t command) {
	struct WriteHandler {
		WriteHandler(const WriteHandler&) = delete;
		WriteHandler& operator=(const WriteHandler&) = delete;
		WriteHandler(WriteHandler&&) = default;
		std::array<uint8_t, 4> buf;
		void operator()(const boost::system::error_code& err, size_t len) {
			if (err) {
				throw std::runtime_error("Send Error");
			}
			std::cout << "Successfully sent " << buf[0] << " " << int(buf[1]) << " " << int(buf[2])
			          << std::endl;
		}
	};

	WriteHandler writeHandler{ { 'x', time, command, '\b' } };
	socket.async_send(boost::asio::buffer(writeHandler.buf), std::move(writeHandler));
}

void Client::handleRecv(std::stringstream sstream) {
	char command;
	sstream.get(command);
	switch (command) {
		case 'c': {
			std::string text;
			std::getline(sstream, text);
			std::string newChatLine = username + ": " + text;
			server.addChatLine(newChatLine);
			std::cout << "Received from " << username << ": " << text << std::endl;
			break;
		}
		case 'p':
			server.startMatchmaking(shared_from_this());
			break;
		case 'x': {
			uint8_t time = sstream.get();
			uint8_t command = sstream.get();
			if (command != 6) { // FIXME: Use enum class
				std::cout << "forwarding command " << int(command) << " at time " << int(time)
				          << std::endl;
			}
			opponent->forward(time, command);
			break;
		}
	}
	asyncReceive(socket, [this](std::stringstream sstream) { handleRecv(std::move(sstream)); });
}
