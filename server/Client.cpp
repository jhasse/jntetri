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

void Client::login(nlohmann::json data) {
	std::string user = data["name"];
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
	socket.send(boost::asio::buffer(std::string("{\"type\": \"ok\"}") + DELIMITER));
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
		if(s == 0) {
			return;
		}
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
