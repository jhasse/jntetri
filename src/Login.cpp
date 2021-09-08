#include "Login.hpp"

#include "../server/NetworkConstants.hpp"
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

const std::string Login::server_("127.0.0.1"); //("85.214.187.23");
const int Login::port_ = 7070;

Login::Login(std::shared_ptr<MultiplayerMenu> multiplayerMenu)
: menu_(multiplayerMenu), text_("connecting ..."), cancel_("Cancel", [this]() { OnCancel(); }),
  socket_(new Socket) {
	spdlog::info("Connecting to {}:{}", server_, port_);
	socket_->connect(server_, port_, [this]() { HandleConnect(); });
	cancel_.setCenter(0, 200);
}

void Login::HandleConnect() {
	text_ = "sending ...";
	spdlog::info("Sending protocol version {}", PROTOCOL_VERSION);
	socket_->send(std::to_string(PROTOCOL_VERSION), [this]() { ProtocolCheck1(); });
}

void Login::ProtocolCheck1() {
	text_ = "receiving ...";
	spdlog::info("Waiting for server accept connection");
	socket_->receive([this](json data) { ProtocolCheck2(std::move(data)); });
}

void Login::ProtocolCheck2(json temp) {
	if (temp["type"] != "ok") {
		text_ = "Error: ";
		text_ += temp;
		OnError();
	} else {
		socket_->send(json{ { "type", "login" },
		                    { "name", menu_->GetName() },
		                    { "password", menu_->GetPassword() } },
		              [this]() { HandleLogin1(); });
	}
}

void Login::HandleLogin1() {
	socket_->receive([this](json temp) { HandleLogin2(std::move(temp)); });
	text_ = "waiting for authentification ...";
}

void Login::HandleLogin2(json temp) {
	if (temp["type"] == "ok") {
		GoToLobby();
	} else if (temp["type"] == "unknown name") {
		text_ = "No user with this name found.\nDo you want to register yourself?";
		cancel_.setCenter(-350, 880);
		cancel_.SetText("No");
		Button* yes = new Button("Yes", boost::bind(&Login::Register, this));
		yes->setCenter(350, 880);
		addWidget(std::shared_ptr<Widget>(yes));
	} else if (temp["type"] == "wrong password") {
		text_ = "The password you've entered\nis wrong. Please try again.";
	} else {
		text_ = "Error: ";
		text_ += temp;
		OnError();
	}
}

void Login::Register() {
	socket_->send(json{ { "type", "register" },
	                    { "name", menu_->GetName() },
	                    { "password", menu_->GetPassword() } },
	              boost::bind(&Login::HandleRegister1, this));
	cancel_.setCenter(0, 800);
	cancel_.SetText("Cancel");
	widgets_.clear(); // FIXME: Implement RemoveWidget function
}

void Login::HandleRegister1() {
	socket_->receive([this](json temp) { HandleRegister2(std::move(temp)); });
	text_ = "please wait ...";
}

void Login::HandleRegister2(json temp) {
	if (temp["type"] == "ok") {
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
