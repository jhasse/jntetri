#pragma once

#include "engine/work.hpp"
#include "engine/socket.hpp"
#include "gui/Button.hpp"
#include "multiplayermenu.hpp"

#include <boost/asio.hpp>

class Login : public Work {
public:
	Login(std::shared_ptr<MultiplayerMenu>, bool quickLogin);
	void step();
	void draw() const;
	void HandleConnect();
	void ProtocolCheck1();
	void ProtocolCheck2(json);
	void HandleLogin1();
	void HandleLogin2(json);
	void OnCancel();
	void OnError();
	void Register();
	void HandleRegister1();
	void HandleRegister2(json);
	void GoToLobby(std::string username);

private:
	std::shared_ptr<MultiplayerMenu> menu;
	std::string text_;
	const static std::string server_;
	const static int port_;
	const static std::string protocolVersion_;
	Button cancel_;
	std::shared_ptr<Socket> socket_;
	bool quickLogin;
};
