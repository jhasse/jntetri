#include "Login.hpp"

#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "gui/Button.hpp"
#include "lobby.hpp"

#include <jngl/all.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <spdlog/spdlog.h>

const std::string Login::server_("85.214.187.23");
const int Login::port_ = 7070;
const std::string Login::protocolVersion_ = "1";

Login::Login(std::shared_ptr<MultiplayerMenu> multiplayerMenu)
: menu_(multiplayerMenu), text_("connecting ..."), cancel_("Cancel", [this]() { OnCancel(); }),
  socket_(new Socket) {
	spdlog::info("Connecting to {}:{}", server_, port_);
	socket_->connect(server_, port_, [this]() { HandleConnect(); });
	cancel_.setCenter(0, 200);
}

void Login::HandleConnect() {
	text_ = "sending ...";
	spdlog::info("Sending protocol version {}", protocolVersion_);
	socket_->send(protocolVersion_, [this]() { ProtocolCheck1(); });
}

void Login::ProtocolCheck1() {
	text_ = "receiving ...";
	spdlog::info("Waiting for server accept connection");
	socket_->receive([this](std::string err) { ProtocolCheck2(err); });
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
		socket_->send(sstream.str(), [this]() { HandleLogin1(); });
	}
}

void Login::HandleLogin1() {
	socket_->receive([this](std::string temp) { HandleLogin2(std::move(temp)); });
	text_ = "waiting for authentification ...";
}

void Login::HandleLogin2(std::string temp) {
	if(temp == "ok") {
		GoToLobby();
	}
	else if(temp == "unknown name") {
		text_ = "No user with this name found.\nDo you want to register yourself?";
		cancel_.setCenter(-350, 880);
		cancel_.SetText("No");
		Button* yes = new Button("Yes", boost::bind(&Login::Register, this));
		yes->setCenter(350, 880);
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
	socket_->send(sstream.str(), boost::bind(&Login::HandleRegister1, this));
	cancel_.setCenter(0, 800);
	cancel_.SetText("Cancel");
	widgets_.clear(); // FIXME: Implement RemoveWidget function
}

void Login::HandleRegister1() {
	socket_->receive([this](std::string temp) { HandleRegister2(std::move(temp)); });
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
		socket_->step();
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
	GetScreen().printCentered(text_, { 0, -150 });
	cancel_.draw();
	DrawWidgets();
}

void Login::OnCancel() {
	jngl::setWork(menu_);
}

void Login::OnError() {
	cancel_.SetText("OK");
}
