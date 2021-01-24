#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <functional>

class Socket : boost::noncopyable {
public:
	Socket();
	void Connect(const std::string& server, int port, std::function<void()> onSuccess);
	void Send(const std::string& data, std::function<void()> onSuccess);
	void Receive(std::function<void(std::string)> onSuccess);
	void Step();

private:
	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_;
	void CheckBuffer(std::string&);
	void ReceiveWrapper(const boost::system::error_code& err, size_t,
	                    std::function<void(std::string)>);
	boost::array<char, 1024> buf_;
	boost::asio::streambuf streamBuffer_;
	std::string tempBuffer_;
};
