#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <functional>
#include <nlohmann/json.hpp>

using nlohmann::json;

class Socket : boost::noncopyable {
public:
	Socket();
	void connect(const std::string& server, int port, std::function<void()> onSuccess);
	void send(const std::string& data, std::function<void()> onSuccess);
	void send(const json& data, std::function<void()> onSuccess);
	void receive(std::function<void(json)> onSuccess);
	void step();

private:
	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_;
	void CallbackWrapper(const boost::system::error_code& err, std::function<void()> onSuccess);
	std::string buffer;
	std::array<char, 1024> receiveBuffer;

	std::function<void(json)> onReceiveSuccess;
};
