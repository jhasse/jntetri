#include "login.hpp"
#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "lobby.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

const std::string Login::server_("tsrom.dyndns.org");
const int Login::port_ = 7070;
const std::string Login::protocolVersion_ = "1";

template<class T>
void Send(boost::asio::ip::tcp::socket& socket, const std::string& text, T callback)
{
	socket.async_send(boost::asio::buffer(text + "\r\n"), callback);
}

Login::Login(boost::shared_ptr<MultiplayerMenu> multiplayerMenu)
	: menu_(multiplayerMenu), text_("connecting ..."),
	  cancel_("Cancel", boost::bind(&Login::OnCancel, this)),
	  socket_(new Socket)
{
	socket_->Connect(server_, port_, boost::bind(&Login::HandleConnect, this));
	cancel_.CenterAt(0, 800);
}

void Login::HandleConnect()
{
	text_ = "sending ...";
	socket_->Send(protocolVersion_, boost::bind(&Login::ProtocolCheck1, this));
}

void Login::ProtocolCheck1()
{
	text_ = "receiving ...";
	socket_->Receive(boost::bind(&Login::ProtocolCheck2, this, _1));
}

void Login::ProtocolCheck2(std::string temp)
{
	if(temp != "ok")
	{
		text_ = "Error: ";
		text_ += temp;
		OnError();
	}
	else
	{
		std::stringstream sstream;
		sstream << "login\n" << menu_->GetName() << "\n" << menu_->GetPassword();
		socket_->Send(sstream.str(), boost::bind(&Login::HandleLogin1, this));
	}
}

void Login::HandleLogin1()
{
	socket_->Receive(boost::bind(&Login::HandleLogin2, this, _1));
	text_ = "waiting for authentification ...";
}

void Login::HandleLogin2(std::string temp)
{
	if(temp == "ok")
	{
		GoToLobby();
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
		text_ = "Error: ";
		text_ += temp;
		OnError();
	}
}

void Login::Register()
{
	std::stringstream sstream;
	sstream << "register\n" << menu_->GetName() << "\n" << menu_->GetPassword() << "\r\n";
	socket_->Send(sstream.str(), boost::bind(&Login::HandleRegister1, this));
	cancel_.CenterAt(0, 800);
	cancel_.SetText("Cancel");
	widgets_.clear(); // FIXME: Implement RemoveWidget function
}

void Login::HandleRegister1()
{
	socket_->Receive(boost::bind(&Login::HandleRegister2, this, _1));
	text_ = "please wait ...";
}

void Login::HandleRegister2(std::string temp)
{
	if(temp == "ok")
	{
		GoToLobby();
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
	socket_->Step();
	cancel_.Step();
	StepWidgets();
}

void Login::GoToLobby()
{
	GetOptions().SetLastLoginName(menu_->GetName());
	GetProcedure().SetWork(new Fade(new Lobby(socket_)));
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
