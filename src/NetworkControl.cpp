#include "NetworkControl.hpp"

#include "engine/socket.hpp"
#include "jngl/window.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

NetworkControl::NetworkControl(std::shared_ptr<Socket> socket) : socket(std::move(socket)) {
	this->socket->receive([this](std::string buf) { handleReceive(buf); });
}

bool NetworkControl::step(const std::function<void(ControlType)>& set) {
	if (!sendQueue.empty() && !sendingInProgress) {
		sendingInProgress = true;
		nlohmann::json j = {
			{ "type", "game" },
			{ "time", sendQueue.front().first },
			{ "control", sendQueue.front().second },
		};
		socket->send(std::string("x") + j.dump(), [this]() {
			sendQueue.pop();
			spdlog::info("sent package success. {} to go.", sendQueue.size());
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

void NetworkControl::handleReceive(std::string buf) {
	if (buf.length() > 1) {
		spdlog::debug("Received: " + buf);
		auto j = nlohmann::json::parse(buf.substr(1) /* remove x */);
		if (j["type"] == "game") {
			uint8_t time = j["time"].get<uint8_t>();
			uint8_t control = j["control"].get<uint8_t>();
			if (control >= static_cast<int>(ControlType::LastValue)) {
				spdlog::error("invalid control type in package: {}", control);
			} else {
				if (control == static_cast<uint8_t>(ControlType::Null)) {
					spdlog::info("Null package received.");
					++nullPackagesReceived;
				}
				data.push(
				    std::pair<unsigned char, ControlType>(time, static_cast<ControlType>(control)));
			}
		} else if (j["type"] == "quit") {
			return; // Don't receive! Back to lobby
		} else {
			spdlog::error("Unknown package: {}", buf);
		}
		socket->receive([this](std::string buf) { handleReceive(buf); });
	}
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
	std::string buf("q");
	socket->send(buf, [this]() {
		spdlog::info("sent quit success.");
	});
}

std::shared_ptr<Socket> NetworkControl::getSocket() {
	return socket;
}
