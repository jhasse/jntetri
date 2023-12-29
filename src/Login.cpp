#include "Login.hpp"

#include "../server/NetworkConstants.hpp"
#include "engine/screen.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "gui/Button.hpp"
#include "lobby.hpp"

#include <jngl.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <spdlog/spdlog.h>

#ifdef __EMSCRIPTEN__
const std::string Login::server_("89.58.48.219"); // boomshine.de
const int Login::port_ = 9999;
#else
// const std::string Login::server_("127.0.0.1");
const std::string Login::server_("89.58.48.219"); // boomshine.de
// const std::string Login::server_("85.214.187.23"); // babynamensuche.de
const int Login::port_ = 7070;
#endif

Login::Login(std::shared_ptr<MultiplayerMenu> multiplayerMenu, bool quickLogin)
: menu(multiplayerMenu), text_("connecting ..."), cancel_("Cancel", [this]() { OnCancel(); }),
  socket_(new Socket), quickLogin(quickLogin) {
	spdlog::info("Connecting to {}:{}", server_, port_);
	try {
		socket_->connect(server_, port_, [this]() { HandleConnect(); });
	} catch(std::exception& e) {
		text_ = "Exception: ";
		text_ += e.what();
		OnError();
	}
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
		if (const std::string msg = temp["msg"]; !msg.empty()) {
			text_ += msg;
		} else {
			text_ += temp.dump();
		}
		OnError();
	} else {
		if (menu->GetName().empty()) {
			if (!menu->GetPassword().empty()) {
				text_ = "Name must not be empty!";
				return OnError();
			}
			socket_->send(json{ { "type", "login_anonymous" } }, [this]() {
				socket_->receive([this](json temp) { HandleLogin2(std::move(temp)); });
				text_ = "waiting for authentification ...";
			});
		} else {
			socket_->send(json{ { "type", "login" },
			                    { "name", menu->GetName() },
			                    { "password", menu->GetPassword() } },
			              [this]() { HandleLogin1(); });
		}
	}
}

void Login::HandleLogin1() {
	socket_->receive([this](json temp) { HandleLogin2(std::move(temp)); });
	text_ = "waiting for authentification ...";
}

void Login::HandleLogin2(json temp) {
	if (temp["type"] == "ok") {
		GoToLobby(temp["name"]);
	} else if (temp["type"] == "unknown name") {
		text_ = "No user with this name found.\nDo you want to register yourself?";
		cancel_.setCenter(-350, 280);
		cancel_.SetText("No");
		Button* yes = new Button("Yes", boost::bind(&Login::Register, this));
		yes->setCenter(350, 280);
		addWidget(std::shared_ptr<Widget>(yes));
	} else if (temp["type"] == "wrong password") {
		text_ = "The password you've entered\nis wrong. Please try again.";
		OnError();
	} else {
		text_ = "Error: ";
		text_ += temp.dump();
		OnError();
	}
}

void Login::Register() {
	json j{ { "type", "register" },
		    { "name", menu->GetName() },
		    { "password", menu->GetPassword() } };
	spdlog::info("Sending: {}", j.dump());
	socket_->send(j, boost::bind(&Login::HandleRegister1, this));
	cancel_.setCenter(0, 200);
	cancel_.SetText("Cancel");
	widgets_.clear(); // FIXME: Implement RemoveWidget function
}

void Login::HandleRegister1() {
	socket_->receive([this](json temp) { HandleRegister2(std::move(temp)); });
	text_ = "please wait ...";
}

void Login::HandleRegister2(json temp) {
	spdlog::debug("HandleRegister2");
	if (temp["type"] == "ok") {
		GoToLobby(temp["name"]);
	} else {
		text_ = "Error: ";
		text_ += temp.dump();
		OnError();
	}
}

void Login::step() {
	try {
		if (socket_) {
			socket_->step();
		}
	} catch(std::exception& e) {
		text_ = "Exception: ";
		text_ += e.what();
		OnError();
	}
	cancel_.step();
	StepWidgets();
}

void Login::GoToLobby(std::string username) {
	getOptions().lastLoginName = std::move(username);
	getOptions().Save();
	jngl::setWork(std::make_shared<Fade>(
	    std::make_shared<Lobby>(std::move(socket_), quickLogin)));
}

void Login::draw() const {
	menu->draw();
	jngl::setColor(255, 255, 255, 220);
	jngl::drawRect(-jngl::getScreenWidth() / 2, -jngl::getScreenHeight() / 2,
	               jngl::getScreenWidth(), jngl::getScreenHeight());
	jngl::setFontColor(0, 0, 0);
	GetScreen().printCentered(text_, { 0, -150 });
	cancel_.draw();
	DrawWidgets();
}

void Login::OnCancel() {
	jngl::setWork(menu);
}

void Login::OnError() {
	// hacked together algorithm to word wrap long error messages:
	size_t pos = 42;
	while (jngl::getTextWidth(text_) > jngl::getScreenWidth() && pos < text_.size()) {
		size_t i = pos;
		while (i > 0 && text_[i - 1] != ' ') {
			if (text_[i] == '\n') {
				i = pos;
				break;
			}
			--i;
		}
		text_.insert(text_.begin() + i, '\n');
		pos += 42;
	}

	cancel_.SetText("OK");
	cancel_.setFocus(true);
}
