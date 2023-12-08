#pragma once

#include "LoginState.hpp"

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
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
	void doAccept(boost::asio::yield_context);
	void addChatLine(boost::asio::yield_context, std::string);
	void startMatchmaking(boost::asio::yield_context, std::shared_ptr<Client>);
	std::string loginAndGetWelcomeMessage(boost::asio::yield_context yield, const std::string& username);

	LoginState checkLogin(std::string username, std::string password);
	bool registerUser(std::string username, std::string password);

	/// creates a new user with a random name with no password
	std::string createAnonymousUser();

private:
	void printStats(const boost::system::error_code&);

	constexpr static auto JNTETRI_PORT = 7070;
	boost::asio::io_service context;
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::deadline_timer timer;

	std::vector<std::shared_ptr<Client>> clients;

	std::string chatText;

	std::mutex matchmakingMutex;
	std::vector<std::shared_ptr<Client>> matchmaking;

	soci::session sql;
};
