#include "Client.hpp"

#include "Server.hpp"

#include <boost/array.hpp>
#include <iostream>

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_context& context, Server& server) : socket(context), server(server) {
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

void Client::run() {
	uint32_t protocolVersion;
	receive(socket) >> protocolVersion;
	std::cout << "Protocol version: " << protocolVersion << std::endl;
	if (protocolVersion != 1) {
		return;
	}
	socket.send(boost::asio::buffer("ok"));
	std::cout << "Sent \"ok\"." << std::endl;
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
			socket.send(boost::asio::buffer("ok"));
			username = user;
			while (auto sstream = receive(socket)) {
				char command;
				sstream.get(command);
				switch (command) {
					case 'c': {
						std::string text;
						std::getline(sstream, text);
						std::string newChatLine = user + ": " + text;
						server.addChatLine(newChatLine);
						std::cout << "Received from " << user << ": " << text << std::endl;
						break;
					}
					case 'p':
						server.startMatchmaking(shared_from_this());
						break;
					case 'x': {
						uint8_t time = sstream.get();
						uint8_t command = sstream.get();
						if (command != 6) { // FIXME: Use enum class
							std::cout << "forwarding command " << int(command) << " at time "
							          << int(time) << std::endl;
						}
						opponent->forward(time, command);
						break;
					}
				}
			}
		} else {
			socket.send(boost::asio::buffer("wrong password"));
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
	// TODO: Switch to async server or handle multi-threading
}
