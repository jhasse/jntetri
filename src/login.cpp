#include "login.hpp"
#include "engine/screen.hpp"
#include "engine/procedure.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

const std::string Login::server_("79.237.169.63");
const int Login::port_ = 7070;
const std::string Login::protocolVersion_ = "1";

template<class T>
void Send(boost::asio::ip::tcp::socket& socket, const std::string& text, T callback)
{
	socket.async_send(boost::asio::buffer(text + "\r\n"), callback);
}

Login::Login(boost::shared_ptr<MultiplayerMenu> multiplayerMenu)
	: menu_(multiplayerMenu), text_("connecting ..."), socket_(io_),
	  cancel_("Cancel", boost::bind(&Login::OnCancel, this))
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(server_), port_);
	socket_.async_connect(endpoint, boost::bind(&Login::HandleConnect, this));
	cancel_.CenterAt(0, 800);
}

void Login::HandleConnect()
{
	text_ = "handshake ...";
	Send(socket_, protocolVersion_, boost::bind(&Login::ProtocolCheck1, this));
}

void Login::ProtocolCheck1()
{
	socket_.async_receive(boost::asio::buffer(buf_), boost::bind(&Login::ProtocolCheck2, this, boost::asio::placeholders::bytes_transferred));
}

void Login::ProtocolCheck2(size_t len)
{
	std::string temp(buf_.begin(), len - 2);
	if(temp != "ok")
	{
		text_ = "Error: ";
		text_ += temp;
		OnError();
	}
	else
	{
		std::stringstream sstream;
		sstream << "login\n" << menu_->GetName() << "\n" << menu_->GetPassword() << "\r\n";
		socket_.async_send(boost::asio::buffer(sstream.str()), boost::bind(&Login::HandleLogin1, this));
	}
}

void Login::HandleLogin1()
{
	socket_.async_receive(boost::asio::buffer(buf_), boost::bind(&Login::HandleLogin2, this, boost::asio::placeholders::bytes_transferred));
	text_ = "waiting for authentification ...";
}

void Login::HandleLogin2(size_t len)
{
	std::string temp(buf_.begin(), len - 2);
	if(temp == "ok")
	{
	}
	else if(temp == "unknown name")
	{
		text_ = "No user with this name found.\nDo you want to register yourself?";
		cancel_.CenterAt(-350, 880);
		cancel_.SetText("No");
		Button* yes = new Button("Yes", boost::bind(&Login::Register, this));
		yes->CenterAt(350, 880);
		AddWidget(boost::shared_ptr<Widget>(yes));
	}
	else if(temp == "wrong password")
	{
		text_ = "The password you've entered\nis wrong. Please try again.";
	}
	else
	{
		text_ = temp;
	}
}

void Login::Register()
{
	std::stringstream sstream;
	sstream << "register\n" << menu_->GetName() << "\n" << menu_->GetPassword() << "\r\n";
	socket_.async_send(boost::asio::buffer(sstream.str()), boost::bind(&Login::HandleRegister1, this));
	cancel_.CenterAt(0, 800);
	cancel_.SetText("Cancel");
	widgets_.clear(); // FIXME: Implement RemoveWidget function
}

void Login::HandleRegister1()
{
	socket_.async_receive(boost::asio::buffer(buf_), boost::bind(&Login::HandleRegister2, this, boost::asio::placeholders::bytes_transferred));
	text_ = "please wait ...";
}

void Login::HandleRegister2(size_t len)
{
	std::string temp(buf_.begin(), len - 2);
	if(temp == "ok")
	{
		text_ = "Success!";
	}
	else
	{
		text_ = "Error: ";
		text_ += temp;
		OnError();
	}
}

void Login::Step()
{
	io_.poll();
	cancel_.Step();
	StepWidgets();
}

void Login::Draw() const
{
	menu_->Draw();
	jngl::SetColor(255, 255, 255, 220);
	GetScreen().DrawRect(-GetScreen().GetWidth() / 2, 0, GetScreen().GetWidth(), GetScreen().GetHeight());
	jngl::SetFontColor(0, 0, 0);
	GetScreen().PrintCentered(text_, 0, 420);
	cancel_.Draw();
	DrawWidgets();
}

void Login::OnCancel()
{
	GetProcedure().SetWork(menu_);
}

void Login::OnError()
{
	cancel_.SetText("OK");
}
