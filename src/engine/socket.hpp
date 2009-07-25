#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

class Socket : boost::noncopyable {
public:
	Socket();
	void Connect(const std::string& server, int port, boost::function<void()> onSuccess);
	void Send(const std::string& data, boost::function<void()> onSuccess);
	void Receive(boost::function<void(std::string)> onSuccess);
	void Step();
private:
	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_;
	void CheckBuffer(std::string&);
	void ReceiveWrapper(const boost::system::error_code& err, size_t, boost::function<void(std::string)>);
	boost::array<char, 1024> buf_;
	boost::asio::streambuf streamBuffer_;
	std::string tempBuffer_;
};
