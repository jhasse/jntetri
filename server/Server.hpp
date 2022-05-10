#pragma once

#include "LoginState.hpp"

#include <boost/asio.hpp>
#include <map>
#include <mutex>
#include <soci/soci.h>
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
	void printStats(const boost::system::error_code&);

	constexpr static auto JNTETRI_PORT = 7070;
	std::vector<std::thread> threads;
	boost::asio::io_service context;
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::deadline_timer timer;

	std::mutex clientsMutex;
	std::vector<std::shared_ptr<Client>> clients;

	std::mutex chatTextMutex;
	std::string chatText;

	std::mutex matchmakingMutex;
	std::vector<std::shared_ptr<Client>> matchmaking;

	soci::session sql;
};
