#include "NetworkControl.hpp"

#include "engine/socket.hpp"

#include <spdlog/spdlog.h>

NetworkControl::NetworkControl(std::shared_ptr<Socket> socket) : socket(std::move(socket)) {
	this->socket->receive([this](std::string buf) { handleReceive(buf); });
}

bool NetworkControl::step(const std::function<void(ControlType)>& set) {
	socket->step();

	while (!data.empty() && time == data.front().first) {
		set(data.front().second); // execute command
		data.pop();
	}
	if (data.empty()) {
		return false;
	}

	if (time == BUFFER_LENGTH) {
		time = 0;
		if (data.front().second != ControlType::Null) {
			spdlog::error("Received invalid Null package.");
		}
		data.pop();
	} else {
		++time;
	}
	return true;
}

void NetworkControl::handleReceive(std::string buf) {
	if (buf.length() > 0) {
		char actionType = buf[0];
		buf = buf.substr(1);
		switch (actionType) {
			case 'x': {
				if (buf.length() != 3) {
					spdlog::error("invalid package: {}", buf);
					break;
				}
				uint8_t time = buf[1];
				uint8_t control = buf[1];
				if (control >= static_cast<int>(ControlType::LastValue)) {
					spdlog::error("invalid control type in package: {}", control);
					break;
				}
				data.push(
				    std::pair<unsigned char, ControlType>(time, static_cast<ControlType>(control)));
				break;
			}
			default:
				spdlog::error("Unknown package: {}", buf);
				break;
		}
		socket->receive([this](std::string buf) { handleReceive(buf); });
	}
}

void NetworkControl::stepSend(Control& control) {
	control.forEach(
	    [this](ControlType c) { sendQueue.push(std::pair<uint8_t, ControlType>(sendTime, c)); });

	if (sendTime == BUFFER_LENGTH) {
		sendQueue.push(std::pair<uint8_t, ControlType>(0, ControlType::Null));
		sendTime = 0;
	} else {
		++sendTime;
	}

	if (!sendQueue.empty() && !sendingInProgress) {
		sendingInProgress = true;
		std::string buf("x");
		buf += uint8_t(sendQueue.front().first);
		buf += uint8_t(sendQueue.front().second);
		socket->send(buf, [this]() {
			sendQueue.pop();
			spdlog::debug("sent package success.");
			sendingInProgress = false;
		});
	}
}
