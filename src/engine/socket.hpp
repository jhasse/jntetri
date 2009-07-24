#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

class Socket : boost::noncopyable {
public:
	Socket();
	void Connect(const std::string& server, int port, boost::function<void()> onSuccess);
	void Step();
private:
	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_;
};
