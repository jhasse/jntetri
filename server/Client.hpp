#pragma once

#include <boost/asio.hpp>
#include <memory>

class Server;

class Client : public std::enable_shared_from_this<Client> {
public:
	Client(Server&);
	void run();
	boost::asio::ip::tcp::socket& getSocket();
	void setOpponent(std::shared_ptr<Client>);
	std::string getUsername() const;
	void forward(uint8_t time, uint8_t command);

private:
	void handleRecv(std::stringstream);

	boost::asio::io_context context;
	boost::asio::ip::tcp::socket socket;
	Server& server;
	std::shared_ptr<Client> opponent;
	std::string username;
};
