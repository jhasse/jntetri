#include "Server.hpp"

#include "Client.hpp"

#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

Server::Server() : socket(context), acceptor(context, tcp::endpoint(tcp::v4(), JNTETRI_PORT)) {
}

Server::~Server() {
	socket.close();
	for (auto& thread : threads) {
		thread.join();
	}
}

void Server::run() {
	startAccept();
	context.run();
}

void Server::handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& error) {
	std::cout << "New connection." << std::endl;
	if (!error) {
		clients.emplace_back(client);
		threads.emplace_back([client]() {
			try {
				client->run();
			} catch (std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		});
	}
	startAccept();
}

void Server::startAccept() {
	auto newClient = std::make_shared<Client>(context, *this);
	acceptor.async_accept(
	    newClient->getSocket(),
	    boost::bind(&Server::handleAccept, this, newClient, boost::asio::placeholders::error));
}

void Server::addChatLine(std::string line) {
	std::lock_guard<std::mutex> lock(chatTextMutex);
	chatText += line;
	for (const auto& client : clients) {
		client->getSocket().async_send(boost::asio::buffer("c" + line + "\b"),
		                               [](const boost::system::error_code& err, size_t bytesSent) {
			                               if (err) {
				                               std::cerr << "Couldn't send update to client."
				                                         << std::endl;
			                               } else {
				                               std::cout << "Sent " << bytesSent << " bytes."
				                                         << std::endl;
			                               }
		                               });
	}
}
