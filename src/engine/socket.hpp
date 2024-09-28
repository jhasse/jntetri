#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <nlohmann/json.hpp>

using nlohmann::json;

class Socket {
public:
	Socket();
	Socket(const Socket&) = delete;
	Socket(Socket&&) = delete;
	Socket& operator=(const Socket&) = delete;
	Socket& operator=(Socket&&) = delete;
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

	std::function<void(json)> onReceiveSuccess;
};
