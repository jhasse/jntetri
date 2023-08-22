#pragma once

#include <utility>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

class Socket {
public:
	explicit Socket(boost::asio::ip::tcp::socket);

	std::string receive(boost::asio::yield_context);
	void send(boost::asio::yield_context, const std::string&);
	void cancel();

private:
	boost::asio::ip::tcp::socket socket;

	std::string buffer;
};
