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
	commands["login"] = {false, std::bind(&Client::login, this, std::placeholders::_1)};
	commands["chat"] = {true, std::bind(&Client::chat, this, std::placeholders::_1)};
	commands["game"] = {true, std::bind(&Client::game, this, std::placeholders::_1)};
	commands["play"] = {true, std::bind(&Client::play, this, std::placeholders::_1)};
	commands["register"] = {false, std::bind(&Client::register_user, this, std::placeholders::_1)};
}

tcp::socket& Client::getSocket() {
	return socket;
}

void Client::login(nlohmann::json data) {
	std::string user = data["name"];
	std::string password = data["password"];
	std::cout << "Logging in '" << user << "' with password of length " << password.size()
	          << std::endl;
	switch(server.checkLogin(user, password)) {
		case UserDoesNotExist:
			errAndDisconnect("unknown name", "", false);
			break;
		case PasswordOK:
			std::cout << "Accepted password, sending \"ok\\b\" ..." << std::endl;
			username = user;
			okMsg();
			break;
		case PasswordWrong:
			errAndDisconnect("wrong password", "", false);
			break;
	}
}

void Client::register_user(nlohmann::json data) {
	std::string user = data["name"];
	std::string pw = data["password"];

	if(server.registerUser(username, pw)) {
		okMsg();
		username = user;
	} else {
		errAndDisconnect("error", "Something went wrong during register!");
	}
}

void Client::chat(nlohmann::json data) {
	std::string newChatLine = username + ": " + data["text"].get<std::string>();
	server.addChatLine(newChatLine);
	std::cout << "Received from " << username << ": " << data["text"] << std::endl;
}

void Client::play(nlohmann::json data) {
	server.startMatchmaking(shared_from_this());
}

void Client::sendStartGame() {
	socket.send(boost::asio::buffer(std::string("{\"type\": \"play\"}") + DELIMITER));
}

void Client::sendChatLine(std::string line) {
	auto j = nlohmann::json { {"type", "chat"}, {"text", line} };
	socket.send(boost::asio::buffer(j.dump() + DELIMITER));
}

void Client::game(nlohmann::json data) {
	opponent->forward(data["time"].get<uint8_t>(), data["control"].get<uint8_t>());
}

void Client::okMsg() {
	socket.send(boost::asio::buffer(std::string("{\"type\": \"ok\"}") + DELIMITER));
	std::cout << "Sent \"ok\"." << std::endl;
}

void Client::errAndDisconnect(std::string type, std::string msg, bool really_disconnect) {
	auto j = nlohmann::json { {"type", type }};
	if(msg != "") {
		j["msg"] = msg;
	}
	socket.send(boost::asio::buffer(j.dump() + DELIMITER));
	std::cout << "Sent " << j.dump() << std::endl;
	if(really_disconnect) {
		running = false;
	}
}

void Client::run() {
	uint32_t protocolVersion;
	std::size_t s = read_until(socket, data_received, "\n");
	std::istream is(&data_received);
	is >> protocolVersion;
	is.ignore();
	std::cout << "Protocol version: " << protocolVersion << std::endl;
	if (protocolVersion != PROTOCOL_VERSION) {
		errAndDisconnect("error", "Protocol version mismatch!");
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
			auto current_cmd = commands.find(data["type"]);
			if(current_cmd == commands.end()) {
				errAndDisconnect("error", "Unknown command!");
			} else if (current_cmd->second.first && username == "") {
				errAndDisconnect("error", "You need to be logged in to use this command!");
			} else {
				current_cmd->second.second(data);
			}
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
