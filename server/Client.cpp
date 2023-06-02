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

Client::Client(Server& server, boost::asio::ip::tcp::socket socket)
: socket(std::move(socket)), server(server) {
	commands["login"] = {false, std::bind(&Client::login, this, std::placeholders::_1, std::placeholders::_2)};
	commands["chat"] = {true, std::bind(&Client::chat, this, std::placeholders::_1, std::placeholders::_2)};
	commands["game"] = {true, std::bind(&Client::game, this, std::placeholders::_1, std::placeholders::_2)};
	commands["play"] = {true, std::bind(&Client::play, this, std::placeholders::_1, std::placeholders::_2)};
	commands["register"] = {false, std::bind(&Client::register_user, this, std::placeholders::_1, std::placeholders::_2)};
	commands["quit"] = {true, std::bind(&Client::quit, this, std::placeholders::_1, std::placeholders::_2)};
}

void Client::login(boost::asio::yield_context yield, nlohmann::json data) {
	std::string user = data["name"];
	std::string password = data["password"];
	spdlog::info("login attempt from '{}' with password of length {}", user, password.size());
	switch(server.checkLogin(user, password)) {
		case UserDoesNotExist:
			errAndDisconnect(yield, "unknown name", "", false);
			break;
		case PasswordOK: {
			createLogger("client " + user);
			log().info("accepted password, sending \"ok\" ...");
			const auto msg = server.loginAndGetWelcomeMessage(yield, user);
			username = user;
			okMsg(yield);
			sendChatLine(yield, msg);
			break;
		}
		case PasswordWrong:
			errAndDisconnect(yield, "wrong password", "", false);
			break;
	}
}

void Client::register_user(boost::asio::yield_context yield, nlohmann::json data) {
	std::string user = data["name"];
	std::string pw = data["password"];

	if(server.registerUser(user, pw)) {
		const auto msg = server.loginAndGetWelcomeMessage(yield, user);
		okMsg(yield);
		username = user;
		sendChatLine(yield, msg);
	} else {
		errAndDisconnect(yield, "error", "Something went wrong during register!");
	}
}

void Client::chat(boost::asio::yield_context yield, nlohmann::json data) {
	std::string newChatLine = username + ": " + data["text"].get<std::string>();
	server.addChatLine(yield, newChatLine);
	log().trace("chat: {}", data["text"]);
}

void Client::play(boost::asio::yield_context yield, nlohmann::json data) {
	server.startMatchmaking(yield, shared_from_this());
}

void Client::quit(boost::asio::yield_context yield, nlohmann::json) {
	if (opponent) {
		opponent->sendOpponentQuit(yield);
		opponent.reset();
	}
}

void Client::sendStartGame(boost::asio::yield_context yield) {
	socket.send(yield, "{\"type\": \"play\"}");
}

void Client::sendChatLine(boost::asio::yield_context yield, std::string line) {
	if (username.empty() /* not logged in yet? */) {
		return;
	}
	auto j = nlohmann::json { {"type", "chat"}, {"text", line} };
	socket.send(yield, j.dump());
}

void Client::sendOpponentQuit(boost::asio::yield_context yield) {
	auto j = nlohmann::json { {"type", "opponentQuit"} };
	socket.send(yield, j.dump());
}

void Client::game(boost::asio::yield_context yield, nlohmann::json data) {
	if (!opponent) {
		spdlog::error("Can't process \"game\" without opponent!");
		return;
	}
	try {
		opponent->forward(yield, data["time"].get<uint8_t>(), data["control"].get<uint8_t>());
	} catch(boost::system::system_error& e) {
		log().warn("opponent {} disconnected: {}", opponent->getUsername(), e.what());
		opponent.reset();
		socket.send(yield, "{\"type\": \"disconnected\"}");
	}
}

void Client::okMsg(boost::asio::yield_context yield) {
	socket.send(yield, "{\"type\": \"ok\"}");
	log().trace("sent \"ok\"");
}

void Client::errAndDisconnect(boost::asio::yield_context yield, std::string type, std::string msg,
                              bool really_disconnect) {
	auto j = nlohmann::json { {"type", type }};
	if(msg != "") {
		j["msg"] = msg;
	}
	socket.send(yield, j.dump());
	log().trace("sent {}", j.dump());
	if(really_disconnect) {
		running = false;
	}
}

void Client::run(boost::asio::yield_context yield) {
	uint32_t protocolVersion;
	std::istringstream is(socket.receive(yield));
	is >> protocolVersion;
	if (protocolVersion != PROTOCOL_VERSION) {
		log().error("protocol version: ", protocolVersion);
		errAndDisconnect(yield, "error", "Protocol version mismatch!");
		return;
	}
	okMsg(yield);

	while(running) {
		std::string line = socket.receive(yield);
		auto data = nlohmann::json::parse(line);
		log().trace("got data {}", data.dump());
		auto current_cmd = commands.find(data["type"]);
		if(current_cmd == commands.end()) {
			errAndDisconnect(yield, "error", "Unknown command!");
		} else if (current_cmd->second.first && username == "") {
			errAndDisconnect(yield, "error", "You need to be logged in to use this command!");
		} else {
			current_cmd->second.second(yield, data);
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

void Client::forward(boost::asio::yield_context yield, uint8_t time, uint8_t command) {
	nlohmann::json j = {
		{ "type", "game" },
		{ "control", command },
		{ "time", time },
	};

	socket.send(yield, j.dump());
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
