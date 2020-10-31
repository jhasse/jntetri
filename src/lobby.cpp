#include "lobby.hpp"

#include "engine/fade.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "gui/Button.hpp"
#include "multiplayermenu.hpp"
#include "NetworkControl.hpp"
#include "SplitScreen.hpp"

#include <boost/bind.hpp>
#include <jngl/all.hpp>
#include <spdlog/spdlog.h>

Lobby::Lobby(std::shared_ptr<Socket> socket)
: socket_(socket), chatText_(""), input_(new Input(-700, 500)) {
	logout_.reset(new Button("Logout", [this]() { OnLogout(); }));
	play_.reset(new Button("Play!™", [this]() { OnPlay(); }));
	HandleReceive("");
	input_->setMaxWidth(2500);
	addWidget(input_);
	addWidget(logout_);
	addWidget(play_);
	logout_->setCenter(-450, -450);
	play_->setCenter(450, -450);
}

void Lobby::OnLogout() {
	jngl::setWork(std::make_shared<Fade>(std::make_shared<MultiplayerMenu>()));
}

void Lobby::OnPlay() {
	play_->setSensitive(false);
	socket_->Send("p", []() { spdlog::info("Successfully sent 'p'."); });
}

void Lobby::step() {
	socket_->Step();
	if (jngl::keyPressed(jngl::key::Return)) {
		socket_->Send(std::string("c") + input_->getText(), [this]() { OnMessageSent(); });
		input_->setSensitive(false);
	}
	StepWidgets();
}

void Lobby::OnMessageSent() {
	input_->setText("");
	input_->setSensitive(true);
}

void Lobby::draw() const {
	jngl::setFontColor(0, 0, 0);
	jngl::setFontSize(35);
	jngl::print(chatText_, -700, -250);
	DrawWidgets();
}

void Lobby::HandleReceive(std::string buf) {
	if (buf.length() > 0) {
		char actionType = buf[0];
		buf = buf.substr(1);
		switch (actionType) {
			case 'c': {
				chatText_ += buf;
				chatText_ += '\n';
				int lineCount = 0;
				size_t pos = 0;
				while ((pos = chatText_.find_first_of("\n", pos + 1)) != std::string::npos) {
					++lineCount;
				}
				if (lineCount > 8) {
					pos = chatText_.find_first_of("\n");
					chatText_ = chatText_.substr(pos + 1);
				}
				break;
			}
			case 'p': {
				// Matchmaking was successful and an opponent found. Let's start the game.
				auto control = std::make_shared<NetworkControl>(socket_);
				jngl::setWork(std::make_shared<Fade>(std::make_shared<SplitScreen>(control)));
				break;
			}
		}
	}
	socket_->Receive([this](std::string buf) { HandleReceive(buf); });
}
