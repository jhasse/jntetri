#include "Login.hpp"

#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "gui/Button.hpp"
#include "lobby.hpp"

#include <jngl/all.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <spdlog/spdlog.h>

const std::string Login::server_("127.0.0.1");
const int Login::port_ = 7070;
const std::string Login::protocolVersion_ = "1";

template<class T>
void Send(boost::asio::ip::tcp::socket& socket, const std::string& text, T callback) {
	socket.async_send(boost::asio::buffer(text + "\r\n"), callback);
}

Login::Login(std::shared_ptr<MultiplayerMenu> multiplayerMenu)
: menu_(multiplayerMenu), text_("connecting ..."), cancel_("Cancel"), socket_(new Socket) {
	cancel_.Connect([this]() { OnCancel(); });
	spdlog::info("Connecting to {}:{}", server_, port_);
	socket_->Connect(server_, port_, [this]() { HandleConnect(); });
	cancel_.CenterAt(0, 200);
}

void Login::HandleConnect() {
	text_ = "sending ...";
	spdlog::info("Sending protocol version {}", protocolVersion_);
	socket_->Send(protocolVersion_, [this]() { ProtocolCheck1(); });
}

void Login::ProtocolCheck1() {
	text_ = "receiving ...";
	spdlog::info("Waiting for server accept connection");
	socket_->Receive([this](std::string err) { ProtocolCheck2(err); });
}

void Login::ProtocolCheck2(std::string temp) {
	if (temp != "ok") {
		text_ = "Error: ";
		text_ += temp;
		OnError();
	} else {
		std::stringstream sstream;
		sstream << "login\n" << menu_->GetName() << "\n" << menu_->GetPassword();
		spdlog::info("Sending {}", sstream.str());
		socket_->Send(sstream.str(), [this]() { HandleLogin1(); });
	}
}

void Login::HandleLogin1() {
	socket_->Receive(boost::bind(&Login::HandleLogin2, this, _1));
	text_ = "waiting for authentification ...";
}

void Login::HandleLogin2(std::string temp) {
	if(temp == "ok") {
		GoToLobby();
	}
	else if(temp == "unknown name") {
		text_ = "No user with this name found.\nDo you want to register yourself?";
		cancel_.CenterAt(-350, 880);
		cancel_.SetText("No");
		Button* yes = new Button("Yes", boost::bind(&Login::Register, this));
		yes->CenterAt(350, 880);
		addWidget(std::shared_ptr<Widget>(yes));
	}
	else if (temp == "wrong password") {
		text_ = "The password you've entered\nis wrong. Please try again.";
	} else {
		text_ = "Error: ";
		text_ += temp;
		OnError();
	}
}

void Login::Register() {
	std::stringstream sstream;
	sstream << "register\n" << menu_->GetName() << "\n" << menu_->GetPassword() << "\r\n";
	socket_->Send(sstream.str(), boost::bind(&Login::HandleRegister1, this));
	cancel_.CenterAt(0, 800);
	cancel_.SetText("Cancel");
	widgets_.clear(); // FIXME: Implement RemoveWidget function
}

void Login::HandleRegister1() {
	socket_->Receive(boost::bind(&Login::HandleRegister2, this, _1));
	text_ = "please wait ...";
}

void Login::HandleRegister2(std::string temp) {
	if (temp == "ok") {
		GoToLobby();
	} else {
		text_ = "Error: ";
		text_ += temp;
		OnError();
	}
}

void Login::step() {
	try {
		socket_->Step();
	} catch(std::exception& e) {
		text_ = "Exception: ";
		text_ += e.what();
		OnError();
	}
	cancel_.step();
	StepWidgets();
}

void Login::GoToLobby() {
	getOptions().lastLoginName = menu_->GetName();
	jngl::setWork(std::make_shared<Fade>(std::make_shared<Lobby>(socket_)));
}

void Login::draw() const {
	menu_->draw();
	jngl::setColor(255, 255, 255, 220);
	jngl::drawRect(-jngl::getScreenWidth() / 2, -jngl::getScreenHeight() / 2,
	               jngl::getScreenWidth(), jngl::getScreenHeight());
	jngl::setFontColor(0, 0, 0);
	GetScreen().printCentered(text_, { 0, 0 });
	cancel_.draw();
	DrawWidgets();
}

void Login::OnCancel() {
	jngl::setWork(menu_);
}

void Login::OnError() {
	cancel_.SetText("OK");
}
