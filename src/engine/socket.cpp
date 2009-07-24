#include "socket.hpp"

#include <boost/bind.hpp>

Socket::Socket () : socket_(io_)
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
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(server), port);
	socket_.async_connect(endpoint, boost::bind(CallbackWrapper, boost::asio::placeholders::error, onSuccess));
}
