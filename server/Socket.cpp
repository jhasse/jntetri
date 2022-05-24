#include "Socket.hpp"

// #include <spdlog/spdlog.h>

Socket::Socket(boost::asio::ip::tcp::socket socket) : socket(std::move(socket)) {
}

std::string Socket::receive(boost::asio::yield_context yield) {
	while (true) {
		size_t pos = buffer.find('\n');
		if (pos == std::string::npos) {
			std::array<char, 1024> receiveBuffer{};
			size_t len = socket.async_receive(boost::asio::buffer(receiveBuffer), 0, yield);
			buffer.append(receiveBuffer.data(), len);
			continue;
		}
		auto package = buffer.substr(0, pos);
		// spdlog::trace("received `{}`", package);
		buffer = buffer.substr(pos + 1);
		return package;
	}
}

void Socket::send(boost::asio::yield_context yield, const std::string& package) {
	socket.async_send(boost::asio::buffer(package + "\n"), yield);
}
