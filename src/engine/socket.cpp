#include "socket.hpp"

#include "../../server/NetworkConstants.hpp"

#include <boost/asio/co_spawn.hpp>
#include <jngl.hpp>

Socket::Socket() : socket_(io) {
}

void Socket::step() {
	io.poll();
}

void Socket::CallbackWrapper(const boost::system::error_code& err, std::function<void()> onSuccess) {
	if (err) {
		throw std::runtime_error("socket error");
	}
	boost::asio::co_spawn(io, [this]() -> boost::asio::awaitable<void> {
		while (true) {
			size_t pos = buffer.find('\n');
			if (pos == std::string::npos) {
				std::array<char, 1024> receiveBuffer{};
				size_t len = co_await socket_.async_receive(boost::asio::buffer(receiveBuffer), 0,
				                                            boost::asio::use_awaitable);
				buffer.append(receiveBuffer.data(), len);
				continue;
			}
			auto package = buffer.substr(0, pos);
			// jngl::trace("received `{}`", package);
			buffer = buffer.substr(pos + 1);
			onReceiveSuccess(json::parse(package));
		}
	}, boost::asio::detached);
	onSuccess();
}

void Socket::connect(const std::string& server, int port, std::function<void()> onSuccess) {
	using boost::asio::ip::tcp;
#ifndef __EMSCRIPTEN__
	auto resolver = std::make_shared<tcp::resolver>(io);
	resolver->async_resolve(
	    server, "http",
	    [this, port, resolver, server, onSuccess](const boost::system::error_code& err,
	                                              const tcp::resolver::results_type& results) {
		if (err) {
			std::ostringstream sstream;
			sstream << "resolve error " << err.value() << " [" << err.category().name() << "]";
			throw std::runtime_error(sstream.str());
		}
		std::optional<boost::asio::ip::address> first_address;
		for (auto& result : results) {
			if (!first_address) {
				first_address = result.endpoint().address();
			}
			jngl::trace("resolved {} to {}", server, result.endpoint().address().to_string());
		}
		if (!first_address) {
			throw std::runtime_error(std::format("Could not resolve {}", server));
		}
		tcp::endpoint endpoint(*first_address, port); // TODO: Should try all addresses
		socket_.async_connect(endpoint, [this, onSuccess](const boost::system::error_code& err) {
			CallbackWrapper(err, onSuccess);
		});
	});
#else
	tcp::endpoint endpoint(boost::asio::ip::address::from_string(server), port);
	socket_.async_connect(endpoint, [this, onSuccess](const boost::system::error_code& err) {
		CallbackWrapper(err, onSuccess);
	});
#endif
}

void Socket::send(const std::string& data, std::function<void()> onSuccess) {
	jngl::trace("sending {}", data);
	auto buf = std::make_unique<std::string>(data + "\n");
	auto mutableBuf = boost::asio::buffer(*buf);
	socket_.async_send(mutableBuf, [this, buf = std::move(buf), onSuccess = std::move(onSuccess)](
	                                   const boost::system::error_code& err, size_t) {
		CallbackWrapper(err, std::move(onSuccess));
	});
}

void Socket::send(const json& data, std::function<void()> onSuccess) {
	send(data.dump(), std::move(onSuccess));
}

void Socket::receive(std::function<void(json)> onSuccess) {
	onReceiveSuccess = std::move(onSuccess);
}
