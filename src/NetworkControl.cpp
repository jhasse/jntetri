#include "NetworkControl.hpp"

#include "engine/socket.hpp"
#include "jngl/window.hpp"

#include <spdlog/spdlog.h>

NetworkControl::NetworkControl(std::shared_ptr<Socket> socket) : socket(std::move(socket)) {
	this->socket->receive([this](std::string buf) { handleReceive(buf); });
}

bool NetworkControl::step(const std::function<void(ControlType)>& set) {
	if (!sendQueue.empty() && !sendingInProgress) {
		sendingInProgress = true;
		std::string buf("x");
		buf += uint8_t(sendQueue.front().first);
		buf += uint8_t(sendQueue.front().second);
		socket->send(buf, [this]() {
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
	if (buf.length() > 0) {
		char actionType = buf[0];
		buf = buf.substr(1);
		switch (actionType) {
			case 'x': {
				if (buf.length() != 2) {
					spdlog::error("invalid x package of length {}: {}", buf.length(), buf);
					break;
				}
				uint8_t time = buf[0];
				uint8_t control = buf[1];
				if (control >= static_cast<int>(ControlType::LastValue)) {
					spdlog::error("invalid control type in package: {}", control);
					break;
				}
				if (control == static_cast<uint8_t>(ControlType::Null)) {
					spdlog::info("Null package received.");
					++nullPackagesReceived;
				}
				data.push(
				    std::pair<unsigned char, ControlType>(time, static_cast<ControlType>(control)));
				break;
			}
			case 'q':
				return; // Don't receive! Back to lobby
			default:
				spdlog::error("Unknown package: {}", buf);
				break;
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