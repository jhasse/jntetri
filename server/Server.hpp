#pragma once

#include <boost/asio.hpp>
#include <mutex>

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

private:
	constexpr static auto JNTETRI_PORT = 7070;
	std::vector<std::thread> threads;
	boost::asio::io_service context;
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::acceptor acceptor;
	std::vector<std::shared_ptr<Client>> clients;

	std::mutex chatTextMutex;
	std::string chatText;

	std::mutex matchmakingMutex;
	std::vector<std::shared_ptr<Client>> matchmaking;
};
