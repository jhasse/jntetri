#include "socket.hpp"

#include "../../server/NetworkConstants.hpp"

#include <iostream>
#include <boost/bind/bind.hpp>
#include <spdlog/spdlog.h>

Socket::Socket() : socket_(io_) {
}

void Socket::step() {
	io_.poll();
}

void Socket::CallbackWrapper(const boost::system::error_code& err, std::function<void()> onSuccess) {
	if (err) {
		throw std::runtime_error("socket error");
	} else {
		boost::asio::async_read_until(
		    socket_, streamBuffer_, DELIMITER,
		    [this](const boost::system::error_code& err, size_t len) { ReceiveWrapper(err, len); });
		onSuccess();
	}
}

void Socket::connect(const std::string& server, int port, std::function<void()> onSuccess) {
	using boost::asio::ip::tcp;
	auto resolver = std::make_shared<tcp::resolver>(io_);
	tcp::resolver::query query(server, "http");
	resolver->async_resolve(query, [this, port, resolver,
	                                onSuccess](const boost::system::error_code& err,
	                                           tcp::resolver::iterator endpointIterator) {
		if (err) {
			std::ostringstream sstream;
			sstream << "resolve error " << err.value() << " [" << err.category().name() << "]";
			throw std::runtime_error(sstream.str());
		}
		tcp::endpoint endpoint(endpointIterator->endpoint().address(), port);
		socket_.async_connect(
		    endpoint, boost::bind(&Socket::CallbackWrapper, this, boost::asio::placeholders::error, onSuccess));
	});
}

void Socket::send(const std::string& data, std::function<void()> onSuccess) {
	auto buf = std::make_unique<std::string>(data + DELIMITER);
	auto mutableBuf = boost::asio::buffer(*buf);
	socket_.async_send(mutableBuf, [this, buf = std::move(buf), onSuccess = std::move(onSuccess)](
	                                   const boost::system::error_code& err, size_t) {
		CallbackWrapper(err, std::move(onSuccess));
	});
}

void Socket::receive(std::function<void(std::string)> onSuccess) {
	onReceiveSuccess = std::move(onSuccess);
}

void Socket::ReceiveWrapper(const boost::system::error_code& err, size_t len) {
	if (err) {
		throw std::runtime_error("socket error");
	}
	if (len == 0) {
		spdlog::warn("len == 0");
	} else {
		std::istream is(&streamBuffer_);
		std::string buf;
		std::getline(is, buf, DELIMITER[0]);
		// spdlog::debug("Received: buf.size(): {}, len: {}, buf: {}", buf.size(), len, buf);
		onReceiveSuccess(buf);
	}
	boost::asio::async_read_until(
	    socket_, streamBuffer_, DELIMITER,
	    [this](const boost::system::error_code& err, size_t len) { ReceiveWrapper(err, len); });
}

bool Socket::packageFinished(const std::string& buf) const {
	for (size_t i = 0; i < buf.length(); ++i) {
		if (/*FIXME*/ i > 0 && buf[i - 1] != 'x' && buf[i] == DELIMITER[0]) {
			return true;
		}
	}
	return false;
}
