#pragma once

#include <boost/asio.hpp>
#include <memory>

class Server;

class Client : public std::enable_shared_from_this<Client> {
public:
	Client(boost::asio::io_context& context, Server&);
	void run();
	boost::asio::ip::tcp::socket& getSocket();
	void setOpponent(std::shared_ptr<Client>);

private:
	boost::asio::ip::tcp::socket socket;
	Server& server;
	std::shared_ptr<Client> opponent;
};
