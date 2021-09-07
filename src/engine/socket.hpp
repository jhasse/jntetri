#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <functional>

class Socket : boost::noncopyable {
public:
	Socket();
	void connect(const std::string& server, int port, std::function<void()> onSuccess);
	void send(const std::string& data, std::function<void()> onSuccess);
	void receive(std::function<void(std::string)> onSuccess);
	void step();

private:
	bool packageFinished(const std::string&) const;

	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_;
	void CallbackWrapper(const boost::system::error_code& err, std::function<void()> onSuccess);
	void ReceiveWrapper(const boost::system::error_code& err, size_t);
	boost::array<char, 1024> buf_;
	boost::asio::streambuf streamBuffer_;

	std::function<void(std::string)> onReceiveSuccess;
};
