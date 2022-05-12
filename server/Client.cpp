#include "Client.hpp"

#include "Server.hpp"
#include "NetworkConstants.hpp"

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <thread>

using boost::asio::ip::tcp;

Client::Client(Server& server) : socket(context), server(server) {
	commands["login"] = {false, std::bind(&Client::login, this, std::placeholders::_1)};
	commands["chat"] = {true, std::bind(&Client::chat, this, std::placeholders::_1)};
	commands["game"] = {true, std::bind(&Client::game, this, std::placeholders::_1)};
	commands["play"] = {true, std::bind(&Client::play, this, std::placeholders::_1)};
	commands["register"] = {false, std::bind(&Client::register_user, this, std::placeholders::_1)};
	commands["quit"] = {true, std::bind(&Client::quit, this, std::placeholders::_1)};
}

tcp::socket& Client::getSocket() {
	return socket;
}

void Client::login(nlohmann::json data) {
	std::string user = data["name"];
	std::string password = data["password"];
	spdlog::info("login attempt from '{}' with password of length {}", user, password.size());
	switch(server.checkLogin(user, password)) {
		case UserDoesNotExist:
			errAndDisconnect("unknown name", "", false);
			break;
		case PasswordOK:
			createLogger("client " + user);
			log().info("accepted password, sending \"ok\" ...");
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

	if(server.registerUser(user, pw)) {
		okMsg();
		username = user;
	} else {
		errAndDisconnect("error", "Something went wrong during register!");
	}
}

void Client::chat(nlohmann::json data) {
	std::string newChatLine = username + ": " + data["text"].get<std::string>();
	server.addChatLine(newChatLine);
	log().trace("chat: {}", data["text"]);
}

void Client::play(nlohmann::json data) {
	server.startMatchmaking(shared_from_this());
}

void Client::quit(nlohmann::json) {
	if (opponent) {
		opponent->sendOpponentQuit();
		opponent.reset();
	}
}

void Client::sendStartGame() {
	socket.send(boost::asio::buffer(std::string("{\"type\": \"play\"}") + DELIMITER));
}

void Client::sendChatLine(std::string line) {
	auto j = nlohmann::json { {"type", "chat"}, {"text", line} };
	socket.send(boost::asio::buffer(j.dump() + DELIMITER));
}

void Client::sendOpponentQuit() {
	auto j = nlohmann::json { {"type", "opponentQuit"} };
	socket.send(boost::asio::buffer(j.dump() + DELIMITER));
}

void Client::game(nlohmann::json data) {
	if (!opponent) {
		spdlog::error("Can't process \"game\" without opponent!");
		return;
	}
	try {
		opponent->forward(data["time"].get<uint8_t>(), data["control"].get<uint8_t>());
	} catch(boost::system::system_error& e) {
		log().warn("opponent {} disconnected: {}", opponent->getUsername(), e.what());
		opponent.reset();
		socket.send(boost::asio::buffer(std::string("{\"type\": \"disconnected\"}") + DELIMITER));
	}
}

void Client::okMsg() {
	socket.send(boost::asio::buffer(std::string("{\"type\": \"ok\"}") + DELIMITER));
	log().trace("sent \"ok\"");
}

void Client::errAndDisconnect(std::string type, std::string msg, bool really_disconnect) {
	auto j = nlohmann::json { {"type", type }};
	if(msg != "") {
		j["msg"] = msg;
	}
	socket.send(boost::asio::buffer(j.dump() + DELIMITER));
	log().trace("sent {}", j.dump());
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
	if (protocolVersion != PROTOCOL_VERSION) {
		log().error("protocol version: ", protocolVersion);
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
		log().trace("got data size {}, error: {}", s, code);
		std::istream is(&data_received);
		while(std::getline(is, line)) {
			auto data = nlohmann::json::parse(line);
			log().trace("got data {}", data.dump());
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
	std::cout << "Client " << username << " exiting." << std::endl;
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

void Client::createLogger(const std::string& name) {
	try {
		logger = spdlog::stdout_color_mt(name, spdlog::color_mode::always);
	} catch (spdlog::spdlog_ex&) {
		logger = spdlog::get(name);
	}
}

spdlog::logger& Client::log() {
	if (!logger) {
		createLogger("unnamed client " + std::to_string(ptrdiff_t(this)));
	}
	return *logger;
}
