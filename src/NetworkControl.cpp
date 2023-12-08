#include "NetworkControl.hpp"

#include "engine/socket.hpp"
#include "gui/MessageBox.hpp"
#include "jngl/window.hpp"
#include "jngl/work.hpp"
#include "lobby.hpp"
#include "multiplayermenu.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

NetworkControl::NetworkControl(std::shared_ptr<Socket> socket) : socket(std::move(socket)) {
	this->socket->receive([this](json buf) { handleReceive(std::move(buf)); });
}

bool NetworkControl::step(const std::function<void(ControlType)>& set) {
	if (!sendQueue.empty() && !sendingInProgress) {
		sendingInProgress = true;
		nlohmann::json j = {
			{ "type", "game" },
			{ "time", sendQueue.front().first },
			{ "control", sendQueue.front().second },
		};
		socket->send(j, [this]() {
			sendQueue.pop();
			// spdlog::info("sent package success. {} to go.", sendQueue.size());
			sendingInProgress = false;
		});
	}

	socket->step();
	if (nullPackagesReceived < 1) {
		return false;
	}
	bool handledNullPackage = false;
	while ((time % BUFFER_LENGTH) == data.front().first) {
		if (data.front().second == ControlType::Null) {
			if (handledNullPackage) {
				break; // Don't skip ahead if there are no commands between two null packages
			}
			handledNullPackage = true;
			--nullPackagesReceived;
		} else {
			set(data.front().second); // execute command
		}
		data.pop();
		if (data.empty()) {
			break;
		}
	}
	++time;
	return true;
}

void NetworkControl::handleReceive(json j) {
	if (j["type"] == "game") {
		uint8_t time = j["time"].get<uint8_t>();
		uint8_t control = j["control"].get<uint8_t>();
		if (control >= static_cast<int>(ControlType::LastValue)) {
			spdlog::error("invalid control type in package: {}", control);
		} else {
			if (control == static_cast<uint8_t>(ControlType::Null)) {
				// spdlog::info("Null package received.");
				++nullPackagesReceived;
			}
			data.push(
			    std::pair<unsigned char, ControlType>(time, static_cast<ControlType>(control)));
		}
	} else if (j["type"] == "opponentQuit") {
		jngl::setWork<MessageBox>("Your opponent left.",
		                          std::make_shared<Lobby>(std::move(socket), false));
		return;
	} else if (j["type"] == "disconnected") {
		jngl::setWork<MessageBox>("Your opponent disconnected.",
		                          std::make_shared<Lobby>(std::move(socket), false));
		return;
	} else if (j["type"] == "error") {
		jngl::setWork<MessageBox>(j["msg"], std::make_shared<MultiplayerMenu>(false));
		return;
	} else {
		spdlog::error("Unknown package: {}", j.dump());
	}
	socket->receive([this](json buf) { handleReceive(std::move(buf)); });
}

void NetworkControl::stepSend(Control& control) {
	if (sendTime % BUFFER_LENGTH == 0) {
		sendQueue.push(std::pair<uint8_t, ControlType>(0, ControlType::Null));
	}
	control.forEach([this](ControlType c) {
		sendQueue.push(std::pair<uint8_t, ControlType>(sendTime % BUFFER_LENGTH, c));
	});

	++sendTime;
}

bool NetworkControl::desync() const {
	std::stringstream sstream;
	sstream << "time: " << time << "  sendTime: " << sendTime
	        << "  nullPackagesReceived: " << nullPackagesReceived
	        << "  sendQueue.size(): " << sendQueue.size();
	jngl::setTitle(sstream.str());
	return std::abs(time - sendTime) > 1000;
}

void NetworkControl::sendQuit() {
	socket->send(json{ { "type", "quit" } }, [this]() { spdlog::info("sent quit success."); });
}

std::shared_ptr<Socket> NetworkControl::getSocket() {
	return socket;
}
