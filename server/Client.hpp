#pragma once

#include <boost/asio.hpp>

class Server;

class Client {
public:
	Client(boost::asio::io_context& context, Server&);
	void run();
	boost::asio::ip::tcp::socket& getSocket();

private:
	boost::asio::ip::tcp::socket socket;
	Server& server;
};
