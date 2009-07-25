#pragma once

#include "engine/work.hpp"
#include "engine/socket.hpp"
#include "gui/input.hpp"
#include "gui/button.hpp"

#include <boost/shared_ptr.hpp>

class Lobby : public Work {
public:
	Lobby(boost::shared_ptr<Socket>);
	void Step();
	void Draw() const;
	void HandleReceive(std::string);
	void OnMessageSent();
	void OnLogout();
	void OnPlay();
private:
	boost::shared_ptr<Socket> socket_;
	std::string chatText_;
	boost::shared_ptr<Input> input_;
	boost::shared_ptr<Button> logout_;
	boost::shared_ptr<Button> play_;
};
