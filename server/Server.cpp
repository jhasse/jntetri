#include "Server.hpp"

#include "Client.hpp"
#include "NetworkConstants.hpp"

#include <boost/bind/bind.hpp>
#include <iostream>
#include <soci/sqlite3/soci-sqlite3.h>
#include <spdlog/spdlog.h>
#include <optional>

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
}

void Server::run() {
	boost::asio::spawn(context, [this](boost::asio::yield_context yield) { doAccept(yield); });
	spdlog::info("start");
	context.run();
}

void Server::doAccept(boost::asio::yield_context yield) {
	while (true) {
		boost::asio::ip::tcp::socket socket(context);
		acceptor.async_accept(socket, yield);
		auto client = std::make_shared<Client>(*this, std::move(socket));
		spdlog::info("new connection");
		clients.emplace_back(client);
		boost::asio::spawn(context, [this, client](boost::asio::yield_context yield) {
			try {
				client->run(yield);
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
			const auto username = client->getUsername();
			clients.erase(std::find(clients.begin(), clients.end(), client));
			if (!username.empty()) {
				addChatLine(yield, fmt::format("{} left.", username));
			}
		});
	}
}

void Server::addChatLine(boost::asio::yield_context yield, std::string line) {
	chatText += line;
	for (const auto& client : clients) {
		if (client->isLoggedIn()) {
			client->sendChatLine(yield, line);
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
		if (*realPassword == password) {
			// kick all other devices of this user:
			for (const auto& client : clients) {
				if (client->isLoggedIn() && client->getUsername() == username) {
					client->kick("Someone logged in from another device.");
				}
			}
			return LoginState::PasswordOK;
		}
		return LoginState::PasswordWrong;
	}
	return LoginState::UserDoesNotExist;
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

std::string Server::createAnonymousUser() {
	for (int i = 0; i < 10; ++i) {
		std::string name = "user" + std::to_string(rand() % 999999);
		if (registerUser(name, "")) {
			return name;
		}
		// already exists? Maybe we can login without a password:
		switch (checkLogin(name, "")) {
			case LoginState::PasswordWrong:
				break;
			case LoginState::UserDoesNotExist:
				assert(false);
				break;
			case LoginState::PasswordOK:
				return name;
		}
	}
	throw std::runtime_error("Couldn't generate a random username!");
}

void Server::startMatchmaking(boost::asio::yield_context yield, std::shared_ptr<Client> client) {
	std::lock_guard<std::mutex> lock(matchmakingMutex);
	if (matchmaking.empty()) {
		matchmaking.emplace_back(client);
		addChatLine(yield, client->getUsername() + " started matchmaking.");
	} else {
		for (const auto& existingClient : matchmaking) {
			if (existingClient == client) {
				return; // somehow matchmaking was requested twice
			}
		}
		// Found opponent. Let's send p back to both clients so that the game starts.
		matchmaking.back()->setOpponent(client);
		client->setOpponent(matchmaking.back());
		spdlog::info("matching '{}' and '{}'", matchmaking.back()->getUsername(), client->getUsername());
		const int32_t seed = rand();
		matchmaking.back()->sendStartGame(yield, seed);
		client->sendStartGame(yield, seed);
		matchmaking.pop_back();
	}
}

std::string Server::loginAndGetWelcomeMessage(boost::asio::yield_context yield, const std::string& username) {
	addChatLine(yield, fmt::format("{} joined.", username));
	size_t loggedIn = std::count_if(clients.begin(), clients.end(),
	                                [](const auto& client) { return client->isLoggedIn(); });
	return fmt::format("{} user{} online.", loggedIn, loggedIn == 1 ? "" : "s");
}

void Server::printStats(const boost::system::error_code&) {
	spdlog::trace("connected clients: {}", clients.size());
	timer.expires_from_now(boost::posix_time::seconds(15));
	timer.async_wait([this](const auto& e) { printStats(e); });
}
