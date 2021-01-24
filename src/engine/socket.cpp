#include "socket.hpp"
#include "spdlog/spdlog.h"

#include <iostream>
#include <boost/bind/bind.hpp>

const std::string delimiter = "\b";

Socket::Socket() : socket_(io_) {
}

void Socket::step() {
	io_.poll();
}

void CallbackWrapper(const boost::system::error_code& err, std::function<void()> onSuccess) {
	if (err) {
		throw std::runtime_error("socket error");
	} else {
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
		    endpoint, boost::bind(CallbackWrapper, boost::asio::placeholders::error, onSuccess));
	});
}

void Socket::send(const std::string& data, std::function<void()> onSuccess) {
	auto buf = std::make_unique<std::string>(data + delimiter);
	auto mutableBuf = boost::asio::buffer(*buf);
	socket_.async_send(mutableBuf, [this, buf = std::move(buf), onSuccess = std::move(onSuccess)](
	                                   const boost::system::error_code& err, size_t) {
		CallbackWrapper(err, std::move(onSuccess));
	});
}

void Socket::CheckBuffer(std::string& buf) {
	for (size_t pos = 0; pos < buf.length(); ++pos) {
		if (/*FIXME*/ pos > 0 && buf[pos - 1] != 'x' && buf[pos] == delimiter[0]) {
			receiveBuffer = buf.substr(pos + 1);
			buf = buf.substr(0, pos);
			return;
		}
	}
	assert(false);
}

void Socket::receive(std::function<void(std::string)> onSuccess) {
	if (receiveBuffer.find_first_of(delimiter) == std::string::npos) {
		socket_.async_receive(boost::asio::buffer(buf_),
		                      boost::bind(&Socket::ReceiveWrapper, this,
		                                  boost::asio::placeholders::error,
		                                  boost::asio::placeholders::bytes_transferred, onSuccess));
	} else {
		std::string temp = receiveBuffer;
		CheckBuffer(temp);
		onSuccess(temp);
	}
}

void Socket::ReceiveWrapper(const boost::system::error_code& err, size_t len,
                            std::function<void(std::string)> onSuccess) {
	if (len == 0) {
		spdlog::warn("len == 0");
		return;
	}
	if (err) {
		throw std::runtime_error("socket error");
	}
	spdlog::debug("receiveBuffer increased from {} to {}", receiveBuffer.size(),
	              receiveBuffer.size() + len);
	receiveBuffer += std::string(&buf_[0], len);
	if (!packageFinished(receiveBuffer)) {
		// haven't received a full package yet
		socket_.async_receive(boost::asio::buffer(buf_),
		                      boost::bind(&Socket::ReceiveWrapper, this,
		                                  boost::asio::placeholders::error,
		                                  boost::asio::placeholders::bytes_transferred, onSuccess));
	} else {
		std::string temp = receiveBuffer;
		CheckBuffer(temp);
		onSuccess(temp);
	}
}

bool Socket::packageFinished(const std::string& buf) const {
	for (size_t i = 0; i < buf.length(); ++i) {
		if (/*FIXME*/ i > 0 && buf[i - 1] != 'x' && buf[i] == delimiter[0]) {
			return true;
		}
	}
	return false;
}
