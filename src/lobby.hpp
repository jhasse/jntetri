#pragma once

#include "engine/work.hpp"
#include "engine/socket.hpp"
#include "gui/input.hpp"

#include <memory>

class Button;

class Lobby : public Work {
public:
	/// if quickLogin is set, immediately starts matchmaking
	Lobby(std::shared_ptr<Socket>, bool quickLogin);
	void step();
	void draw() const;
	void handleReceive(json);
	void OnMessageSent();
	void OnLogout();
	void OnPlay();

private:
	void startReceiving();

	std::shared_ptr<Socket> socket_;
	std::string chatText_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<Button> logout_;
	std::shared_ptr<Button> play_;
};
