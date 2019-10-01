#include "NetworkControl.hpp"

#include "engine/socket.hpp"

NetworkControl::NetworkControl(std::shared_ptr<Socket> socket) : socket(std::move(socket)) {
	socket->Receive([this](std::string buf) { handleReceive(buf); });
}

bool NetworkControl::step(const std::function<void(ControlType)>& /*set*/) {
	socket->Step();
	// TODO
	return false;
}

void NetworkControl::handleReceive(std::string /*buf*/) {
	// TODO
	socket->Receive([this](std::string buf) { handleReceive(buf); });
}
