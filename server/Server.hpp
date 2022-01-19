#pragma once

#include "LoginState.hpp"

#include <boost/asio.hpp>
#include <map>
#include <mutex>
#include <thread>

class Client;

class Server {
public:
	Server();
	~Server();
	void run();
	void handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& error);
	void startAccept();
	void addChatLine(std::string);
	void startMatchmaking(std::shared_ptr<Client>);

	LoginState checkLogin(std::string username, std::string password);
	bool registerUser(std::string username, std::string password);

private:
	constexpr static auto JNTETRI_PORT = 7070;
	std::vector<std::thread> threads;
	boost::asio::io_service context;
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::acceptor acceptor;

	std::mutex clientsMutex;
	std::vector<std::shared_ptr<Client>> clients;

	std::mutex chatTextMutex;
	std::string chatText;
	std::map<std::string, std::string> userDB;

	std::mutex matchmakingMutex;
	std::vector<std::shared_ptr<Client>> matchmaking;
};
