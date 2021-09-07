#include "Client.hpp"

#include "Server.hpp"

#include <boost/array.hpp>
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>

#include "NetworkConstants.hpp"

using boost::asio::ip::tcp;

Client::Client(Server& server) : socket(context), server(server) {
}

tcp::socket& Client::getSocket() {
	return socket;
}

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

void Client::asyncReceive(std::function<void(std::string)> handler) {
	// find any unhandled packages
	for (int i = 0; i < receiveBuffer.length(); ++i) {
		if (/*FIXME*/ i > 0 && receiveBuffer[i - 1] != 'x' && receiveBuffer[i] == DELIMITER) {
			std::string package = receiveBuffer.substr(0, i);
			receiveBuffer = receiveBuffer.substr(i + 1);
			std::cout << "Using receiveBuffer." << std::endl;
			handler(package);
			return;
		}
	}

	struct ReadHandler {
		tcp::socket& socket;
		std::unique_ptr<std::array<char, 128>> buf;
		std::function<void(std::string)> handler;
		std::string* clientReceiveBuffer;

		ReadHandler(const ReadHandler&) = delete;
		ReadHandler& operator=(const ReadHandler&) = delete;
		ReadHandler(ReadHandler&&) = default;
		~ReadHandler() {
		}

		void operator()(const boost::system::error_code& err, size_t len) {
			if (err) {
				throw std::runtime_error("Receive Error");
			}
			std::string tmp(&((*buf)[0]), len);
			std::cout << "Received " << len << " bytes:";
			for (size_t i = 0; i < len; ++i) {
				std::cout << " " << int((*buf)[i]);
			}
			std::cout << std::endl;

			// find packages
			int start = 0;
			std::string package;
			for (int i = 0; i < tmp.length(); ++i) {
				if (package.empty() && /*FIXME*/ i > 0 && tmp[i - 1] != 'x' &&
				    tmp[i] == DELIMITER) {
					package = tmp.substr(start, i - start);
					clientReceiveBuffer->clear();
					std::string tmp(&((*buf)[0]), len);
					std::cout << "Found package:";
					for (size_t j = 0; j < i - start; ++j) {
						std::cout << " " << int(package[j]);
					}
					std::cout << std::endl;

				} else {
					*clientReceiveBuffer += tmp[i];
				}
			}
			if (package.empty()) {
				socket.async_receive(boost::asio::buffer(*buf), std::move(*this));
			} else {
				handler(package);
			}
		}
	};

	ReadHandler readHandler{ socket, std::make_unique<std::array<char, 128>>(), std::move(handler),
		                     &receiveBuffer };

	auto mutableBuf = boost::asio::buffer(*readHandler.buf);
	socket.async_receive(mutableBuf, std::move(readHandler));
}

void Client::run() {
	uint32_t protocolVersion;
	receive(socket) >> protocolVersion;
	std::cout << "Protocol version: " << protocolVersion << std::endl;
	if (protocolVersion != PROTOCOL_VERSION) {
		return;
	}
	socket.send(boost::asio::buffer({ 'o', 'k', DELIMITER }));
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
			socket.send(boost::asio::buffer({ 'o', 'k', DELIMITER }));
			username = user;
			asyncReceive([this](std::string sstream) { handleRecv(std::move(sstream)); });
			context.run();
		} else {
			socket.send(boost::asio::buffer("wrong password" + std::string(1, DELIMITER)));
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

	WriteHandler writeHandler{ { 'x', time, command, DELIMITER } };
	socket.async_send(boost::asio::buffer(writeHandler.buf), std::move(writeHandler));
}

void Client::handleRecv(std::string buf) {
	switch (buf[0]) {
		case 'c': {
			std::string text;
			std::stringstream sstream(buf.substr(1));
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
			assert(buf.length() == 3);
			uint8_t time = buf[1];
			uint8_t command = buf[2];
			if (command != 6) { // FIXME: Use enum class
				std::cout << "forwarding command " << int(command) << " at time " << int(time)
				          << std::endl;
			}
			opponent->forward(time, command);
			break;
		}
	}
	asyncReceive([this](std::string sstream) { handleRecv(std::move(sstream)); });
}
