#pragma once

#include "engine/work.hpp"
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
	void ProtocolCheck2(size_t);
	void HandleLogin1();
	void HandleLogin2(size_t);
	void OnCancel();
	void OnError();
	void Register();
	void HandleRegister1();
	void HandleRegister2(size_t);
private:
	boost::shared_ptr<MultiplayerMenu> menu_;
	std::string text_;
	const static std::string server_;
	const static int port_;
	const static std::string protocolVersion_;
	boost::array<char, 1024> buf_;
	Button cancel_;
	boost::asio::io_service io_;
	boost::asio::ip::tcp::socket socket_;
};
