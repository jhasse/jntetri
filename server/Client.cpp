#include "Client.hpp"

#include "Server.hpp"

#include <boost/array.hpp>
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>
#include <boost/bind.hpp>
#include "NetworkConstants.hpp"

using boost::asio::ip::tcp;

Client::Client(Server& server) : socket(context), server(server) {
	commands["login"] = std::bind(&Client::login, this, std::placeholders::_1);
	commands["chat"] = std::bind(&Client::chat, this, std::placeholders::_1);
	commands["game"] = std::bind(&Client::game, this, std::placeholders::_1);
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
			if (buf[i] == DELIMITER[0]) {
				return sstream;
			}
			sstream << buf[i];
		}
	}
}


void Client::asyncReceive(std::function<void(std::string)> handler) {
	// find any unhandled packages
	for (int i = 0; i < receiveBuffer.length(); ++i) {
		if (/*FIXME*/ i > 0 && receiveBuffer[i - 1] != 'x' && receiveBuffer[i] == DELIMITER[0]) {
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

			int start = 0;
			std::string package;
			for (int i = 0; i < tmp.length(); ++i) {
				if (package.empty() && /*FIXME*/ i > 0 && tmp[i - 1] != 'x' &&
				    tmp[i] == DELIMITER[0]) {
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

void Client::login(nlohmann::json data) {
	std::string user = data["user"];
	std::string password = data["password"];
	std::cout << "Logging in '" << user << "' with password of length " << password.size();
	if (password == "asd") {
		std::cout << "Accepted password, sending \"ok\\b\" ..." << std::endl;
		username = user;
		okMsg();
	} else {
		errAndDisconnect("wrong password");
	}
}

void Client::chat(nlohmann::json data) {
	std::string newChatLine = data["username"].get<std::string>() + ": " + data["text"].get<std::string>();
	server.addChatLine(newChatLine);
	std::cout << "Received from " << data["username"] << ": " << data["text"] << std::endl;
	okMsg();
}

void Client::play(nlohmann::json data) {
	server.startMatchmaking(shared_from_this());
}

void Client::game(nlohmann::json data) {
	opponent->forward(data["time"].get<uint8_t>(), data["control"].get<uint8_t>());
}

void Client::okMsg() {
	socket.send(boost::asio::buffer("{\"type\": \"ok\"}\n"));
	std::cout << "Sent \"ok\"." << std::endl;
}

void Client::errAndDisconnect(std::string msg) {
	socket.send(boost::asio::buffer("{\"type\": \"error\"}\n"));
	std::cout << "Sent \"error\" with msg: " << msg << std::endl;
	running = false;
}

void Client::run() {
	uint32_t protocolVersion;
	std::size_t s = read_until(socket, data_received, "\n");
	std::istream is(&data_received);
	is >> protocolVersion;
	is.ignore();
	std::cout << "Protocol version: " << protocolVersion << std::endl;
	if (protocolVersion != PROTOCOL_VERSION) {
		errAndDisconnect("Protocol version mismatch!");
		return;
	}
	okMsg();
	
	while(running) {
		std::string line;
		boost::system::error_code code;
		std::size_t s = read_until(socket, data_received, "\n", code);
		std::cout << "Got data size " << s << " error: " << code << std::endl;
		std::istream is(&data_received);
		while(std::getline(is, line)) {
			auto data = nlohmann::json::parse(line);
			std::cout << "Got data " << data.dump() << std::endl;
			std::cout << "Processing command " << data["type"] << std::endl;
			commands[data["type"]](data);
		}
	}
}

void Client::setOpponent(std::shared_ptr<Client> opponent) {
	this->opponent = std::move(opponent);
}

std::string Client::getUsername() const {
	return username;
}

void Client::forward(uint8_t time, uint8_t command) {
	nlohmann::json j = {
		{ "type", "game" },
		{ "control", command },
		{ "time", time },
	};

	socket.send(boost::asio::buffer(j.dump() + DELIMITER));
}
