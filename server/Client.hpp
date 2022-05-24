#pragma once

#include "Socket.hpp"

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <spdlog/logger.h>

class Server;

class Client : public std::enable_shared_from_this<Client> {
public:
	Client(Server&, boost::asio::ip::tcp::socket);
	void run(boost::asio::yield_context);
	void setOpponent(std::shared_ptr<Client>);
	void sendStartGame(boost::asio::yield_context);
	void sendChatLine(boost::asio::yield_context, std::string line);
	void sendOpponentQuit(boost::asio::yield_context);
	std::string getUsername() const;
	void forward(boost::asio::yield_context, uint8_t time, uint8_t command);
	spdlog::logger& log();

private:
	void handleRecv(std::string);
	void handleData(const boost::system::error_code& e, std::size_t size);

	void login(boost::asio::yield_context, nlohmann::json data);
	void chat(boost::asio::yield_context, nlohmann::json data);
	void play(boost::asio::yield_context, nlohmann::json data);
	void quit(boost::asio::yield_context, nlohmann::json);
	void game(boost::asio::yield_context, nlohmann::json data);
	void register_user(boost::asio::yield_context, nlohmann::json data);

	void okMsg(boost::asio::yield_context);
	void errAndDisconnect(boost::asio::yield_context, std::string type, std::string msg, bool really_disconnect = true);
	bool running = true;

	/// <name, <needsLogin, callback>>
	std::map<std::string, std::pair<bool, std::function<void(boost::asio::yield_context, nlohmann::json)>>> commands;

	Socket socket;
	Server& server;
	std::shared_ptr<Client> opponent;
	std::string username;

	void createLogger(const std::string& name);
	std::shared_ptr<spdlog::logger> logger;
};
