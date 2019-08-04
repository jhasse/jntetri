#pragma once

#include "control.hpp"

class Socket;

class NetworkControl : public ControlBase {
public:
	NetworkControl(std::shared_ptr<Socket> socket);
	bool step(const std::function<void(ControlType)>&) override;
	void handleReceive(std::string buf);

private:
	std::shared_ptr<Socket> socket;
};
