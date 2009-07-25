#include "socket.hpp"

#include <iostream>
#include <boost/bind.hpp>

const std::string delimiter = "\b";

Socket::Socket () : socket_(io_), tempBuffer_("")
{
}

void Socket::Step()
{
	io_.poll();
}

void CallbackWrapper(const boost::system::error_code& err, boost::function<void()> onSuccess)
{
	if(err)
	{
		throw std::runtime_error("socket error");
	}
	else
	{
		onSuccess();
	}
}

void Socket::Connect(const std::string& server, int port, boost::function<void()> onSuccess)
{
	using boost::asio::ip::tcp;
	tcp::resolver resolver(io_);
	tcp::resolver::query query(server, "http");
	tcp::endpoint endpoint(resolver.resolve(query)->endpoint().address(), port);
//	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(server), port);
	socket_.async_connect(endpoint, boost::bind(CallbackWrapper, boost::asio::placeholders::error, onSuccess));
}

void Socket::Send(const std::string& data, boost::function<void()> onSuccess)
{
	socket_.async_send(boost::asio::buffer(data + delimiter), boost::bind(CallbackWrapper, boost::asio::placeholders::error, onSuccess));
}

void Socket::CheckBuffer(std::string& buf)
{
	size_t pos = buf.find_first_of(delimiter);
	if(pos != std::string::npos)
	{
		if(pos + delimiter.size() < buf.size())
		{
			tempBuffer_ = buf.substr(pos + delimiter.size());
		}
		else
		{
			tempBuffer_ = "";
		}
		buf = buf.substr(0, pos);
	}
	else
	{
		tempBuffer_ = "";
	}
}

void Socket::Receive(boost::function<void(std::string)> onSuccess)
{
	if(tempBuffer_ == "")
	{
		socket_.async_receive(boost::asio::buffer(buf_), boost::bind(&Socket::ReceiveWrapper, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, onSuccess));
	}
	else
	{
		std::string temp = tempBuffer_;
		CheckBuffer(temp);
		onSuccess(temp);
	}
};

void Socket::ReceiveWrapper(const boost::system::error_code& err, size_t len, boost::function<void(std::string)> onSuccess)
{
	std::cout << len << std::endl;
	if(len == 0)
	{
		return;
	}
	if(err)
	{
		throw std::runtime_error("socket error");
	}
	else
	{
		std::string temp(buf_.begin(), len - delimiter.size());
		CheckBuffer(temp);
		onSuccess(temp);
	}
}
