#pragma once

#include "control.hpp"

#include <queue>

class Socket;

/// The control for the field of the opponent
class NetworkControl : public ControlBase {
public:
	NetworkControl(std::shared_ptr<Socket> socket);
	bool step(const std::function<void(ControlType)>&) override;
	void handleReceive(std::string buf);

	void stepSend(Control&);

private:
	std::shared_ptr<Socket> socket;
	std::queue<std::pair<unsigned char, ControlType>> data;
	int time = 0;

	std::queue<std::pair<unsigned char, ControlType>> sendQueue;
	int sendTime = 0;

	bool sendingInProgress = false;

	const uint8_t BUFFER_LENGTH = 100;
};
