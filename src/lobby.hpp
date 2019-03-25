#pragma once

#include "engine/work.hpp"
#include "engine/socket.hpp"
#include "gui/input.hpp"

#include <memory>

class Button;

class Lobby : public Work {
public:
	Lobby(std::shared_ptr<Socket>);
	void step();
	void draw() const;
	void HandleReceive(std::string);
	void OnMessageSent();
	void OnLogout();
	void OnPlay();
private:
	std::shared_ptr<Socket> socket_;
	std::string chatText_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<Button> logout_;
	std::shared_ptr<Button> play_;
};
