#include "Server.hpp"

#include "Client.hpp"
#include "NetworkConstants.hpp"

#include <boost/bind/bind.hpp>
#include <iostream>
#include <soci/sqlite3/soci-sqlite3.h>
#include <spdlog/spdlog.h>

using boost::asio::ip::tcp;

Server::Server()
: socket(context), acceptor(context, tcp::endpoint(tcp::v4(), JNTETRI_PORT)),
  timer(context, boost::posix_time::seconds(15)), sql(soci::sqlite3, "jntetri.sqlite") {
	sql << "CREATE TABLE IF NOT EXISTS users ("
	       "username VARCHAR(80) UNIQUE NOT NULL,"
	       "password VARCHAR(256) NOT NULL)";
	timer.async_wait([this](const auto& e) { printStats(e); });
}

Server::~Server() {
	socket.close();
	for (auto& thread : threads) {
		thread.join();
	}
}

void Server::run() {
	startAccept();
	spdlog::info("start");
	context.run();
}

void Server::handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& error) {
	spdlog::info("new connection");
	if (!error) {
		{
			std::lock_guard<std::mutex> lock(clientsMutex);
			clients.emplace_back(client);
		}
		threads.emplace_back([this, client]() {
			try {
				client->run();
			} catch (std::exception& e) {
				client->log().error(e.what());
			}
			client->log().info("disconnected");
			{
				std::lock_guard<std::mutex> lock(matchmakingMutex);
				if (const auto it = std::find(matchmaking.begin(), matchmaking.end(), client);
				    it != matchmaking.end()) {
					matchmaking.erase(it);
				}
			}
			{
				std::lock_guard<std::mutex> lock(clientsMutex);
				clients.erase(std::find(clients.begin(), clients.end(), client));
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
	{
		std::lock_guard<std::mutex> lock(clientsMutex);
		for (const auto& client : clients) {
			client->sendChatLine(line);
		}
	}
}

LoginState Server::checkLogin(std::string username, std::string password) {
	soci::rowset<soci::row> rs =
	    (sql.prepare << "select password from users where username = :username",
	     soci::use(username, "username"));
	std::optional<std::string> realPassword;
	for (const auto& row : rs) {
		assert(!realPassword);
		realPassword = row.get<std::string>(0);
	}
	if (realPassword) {
		return (*realPassword == password) ? PasswordOK : PasswordWrong;
	}
	return UserDoesNotExist;
}

bool Server::registerUser(std::string username, std::string password) {
	try {
		sql << "insert into users (username, password) values(:username, :password)",
		    soci::use(username), soci::use(password);
	} catch (soci::soci_error& e) {
		spdlog::error(e.what());
		return false;
	}
	return true;
}

void Server::startMatchmaking(std::shared_ptr<Client> client) {
	std::lock_guard<std::mutex> lock(matchmakingMutex);
	if (matchmaking.empty()) {
		matchmaking.emplace_back(client);
	} else {
		// Found opponent. Let's send p back to both clients so that the game starts.
		matchmaking.back()->setOpponent(client);
		client->setOpponent(matchmaking.back());
		spdlog::info("matching '{}' and '{}'", matchmaking.back()->getUsername(), client->getUsername());
		matchmaking.back()->sendStartGame();
		client->sendStartGame();
	}
}

void Server::printStats(const boost::system::error_code&) {
	spdlog::trace("connected clients: {}", clients.size());
	timer.expires_from_now(boost::posix_time::seconds(15));
	timer.async_wait([this](const auto& e) { printStats(e); });
}
