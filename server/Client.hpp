#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <spdlog/logger.h>

class Server;

class Client : public std::enable_shared_from_this<Client> {
public:
	Client(Server&);
	void run();
	boost::asio::ip::tcp::socket& getSocket();
	void setOpponent(std::shared_ptr<Client>);
	void sendStartGame();
	void sendChatLine(std::string line);
	void sendOpponentQuit();
	std::string getUsername() const;
	void forward(uint8_t time, uint8_t command);
	spdlog::logger& log();

private:
	void handleRecv(std::string);
	void handleData(const boost::system::error_code& e, std::size_t size);

	void login(nlohmann::json data);
	void chat(nlohmann::json data);
	void play(nlohmann::json data);
	void quit(nlohmann::json);
	void game(nlohmann::json data);
	void register_user(nlohmann::json data);

	void okMsg();
	void errAndDisconnect(std::string type, std::string msg, bool really_disconnect = true);
	bool running = true;

	/// <name, <needsLogin, callback>>
	std::map<std::string, std::pair<bool, std::function<void(nlohmann::json)>>> commands;

	boost::asio::io_service context;
	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf data_received;
	Server& server;
	std::shared_ptr<Client> opponent;
	std::string username;

	/// unhandled bytes from the last receive
	std::string receiveBuffer;

	void createLogger(const std::string& name);
	std::shared_ptr<spdlog::logger> logger;
};
