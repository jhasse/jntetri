#pragma once

#include "engine/work.hpp"
#include "engine/socket.hpp"
#include "gui/Button.hpp"
#include "multiplayermenu.hpp"

#include <boost/asio.hpp>

class Login : public Work {
public:
	Login(std::shared_ptr<MultiplayerMenu>);
	void step();
	void draw() const;
	void HandleConnect();
	void ProtocolCheck1();
	void ProtocolCheck2(std::string);
	void HandleLogin1();
	void HandleLogin2(std::string);
	void OnCancel();
	void OnError();
	void Register();
	void HandleRegister1();
	void HandleRegister2(std::string);
	void GoToLobby();

private:
	std::shared_ptr<MultiplayerMenu> menu_;
	std::string text_;
	const static std::string server_;
	const static int port_;
	const static std::string protocolVersion_;
	Button cancel_;
	std::shared_ptr<Socket> socket_;
};
