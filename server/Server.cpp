#include "Server.hpp"

#include "Client.hpp"
#include "NetworkConstants.hpp"

#include <boost/bind/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

Server::Server() : socket(context), acceptor(context, tcp::endpoint(tcp::v4(), JNTETRI_PORT)) {
	userDB["rechts"] = "asd";
	userDB["links"] = "asd";
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
		threads.emplace_back([this, client]() {
			try {
				client->run();
			} catch (std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
			std::lock_guard<std::mutex> lock(matchmakingMutex);
			if (const auto it = std::find(matchmaking.begin(), matchmaking.end(), client);
			    it != matchmaking.end()) {
				matchmaking.erase(it);
			}
		});
	}
	startAccept();
}

void Server::startAccept() {
	auto newClient = std::make_shared<Client>(*this);
	acceptor.async_accept(
	    newClient->getSocket(),
	    boost::bind(&Server::handleAccept, this, newClient, boost::asio::placeholders::error));
}

void Server::addChatLine(std::string line) {
	std::lock_guard<std::mutex> lock(chatTextMutex);
	chatText += line;
	for (const auto& client : clients) {
		client->sendChatLine(line);
	}
}

LoginState Server::checkLogin(std::string username, std::string password) {
	auto user = userDB.find(username);
	if(user == userDB.end()) {
		return UserDoesNotExist;
	} else if(user->second == password) {
		return PasswordOK;
	} else {
		return PasswordWrong;
	}
}

void Server::startMatchmaking(std::shared_ptr<Client> client) {
	std::lock_guard<std::mutex> lock(matchmakingMutex);
	if (matchmaking.empty()) {
		matchmaking.emplace_back(client);
	} else {
		// Found opponent. Let's send p back to both clients so that the game starts.
		matchmaking.back()->setOpponent(client);
		client->setOpponent(matchmaking.back());
		std::cout << "Matching '" << matchmaking.back()->getUsername() << "' and '"
		          << client->getUsername() << "'." << std::endl;
		matchmaking.back()->sendStartGame();
		client->sendStartGame();
	}	
}
