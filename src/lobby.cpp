#include "lobby.hpp"

#include "engine/fade.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "gui/MessageBox.hpp"
#include "multiplayermenu.hpp"
#include "NetworkControl.hpp"
#include "SplitScreen.hpp"

#include <jngl.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

Lobby::Lobby(std::shared_ptr<Socket> socket)
: socket_(socket), chatText_(""), input_(new Input(-700, 500)) {
	logout_.reset(new Button("Logout", [this]() { OnLogout(); }));
	play_.reset(new Button("Play!™", [this]() { OnPlay(); }));
	startReceiving();
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
	socket_->send(json{ { "type", "play" } }, []() { spdlog::info("Successfully sent 'play'."); });
}

void Lobby::step() {
	try {
		socket_->step();
	} catch (std::exception& e) {
		jngl::setWork(std::make_shared<Fade>(
		    std::make_shared<MessageBox>(e.what(), std::make_shared<MultiplayerMenu>())));
	}
	if (jngl::keyPressed(jngl::key::Return)) {
		nlohmann::json j = {
			{ "type", "chat" },
			{ "text", input_->getText() },
		};
		spdlog::debug("Sending: {}", j.dump());
		socket_->send(j, [this]() { OnMessageSent(); });
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

void Lobby::handleReceive(json buf) {
	if (buf["type"] == "chat") {
		chatText_ += buf["text"];
		chatText_ += '\n';
		int lineCount = 0;
		size_t pos = 0;
		while ((pos = chatText_.find_first_of("\n", pos + 1)) != std::string::npos) {
			++lineCount;
		}
		if (lineCount > 13) {
			pos = chatText_.find_first_of("\n");
			chatText_ = chatText_.substr(pos + 1);
		}
	} else if (buf["type"] == "play") {
		// Matchmaking was successful and an opponent found. Let's start the game.
		spdlog::debug("Starting match making");
		auto control = std::make_shared<NetworkControl>(socket_);
		jngl::setWork(std::make_shared<Fade>(std::make_shared<SplitScreen>(control)));
		return; // move out of Lobby loop
	} else {
		spdlog::warn("Received unknown type: {}", buf);
	}
	startReceiving();
}

void Lobby::startReceiving() {
	socket_->receive([this](json buf) { handleReceive(std::move(buf)); });
}
