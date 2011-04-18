#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include "engine/work.hpp"
#include "engine/socket.hpp"
#include "multiplayermenu.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

class Login : public Work {
public:
	Login(boost::shared_ptr<MultiplayerMenu>);
	void Step();
	void Draw() const;
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
	boost::shared_ptr<MultiplayerMenu> menu_;
	std::string text_;
	const static std::string server_;
	const static int port_;
	const static std::string protocolVersion_;
	Button cancel_;
	boost::shared_ptr<Socket> socket_;
};
