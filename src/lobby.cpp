#include "lobby.hpp"

#include "engine/fade.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "gui/Button.hpp"
#include "multiplayermenu.hpp"

#include <boost/bind.hpp>
#include <jngl/all.hpp>

Lobby::Lobby(std::shared_ptr<Socket> socket)
: socket_(socket), chatText_(""), input_(new Input(-700, 1100)) {
	logout_.reset(new Button("Logout", boost::bind(&Lobby::OnLogout, this)));
	play_.reset(new Button("Play!™", boost::bind(&Lobby::OnPlay, this)));
	HandleReceive("");
	addWidget(input_);
	addWidget(logout_);
	addWidget(play_);
	logout_->CenterAt(-450, 150);
	play_->CenterAt(450, 150);
}

void Lobby::OnLogout() {
	jngl::setWork(std::make_shared<Fade>(std::make_shared<MultiplayerMenu>()));
}

void Lobby::OnPlay() {
	//waiting = true;
}

void Lobby::step() {
	socket_->Step();
	if (jngl::keyPressed(jngl::key::Return)) {
		socket_->Send(std::string("c") + input_->getText(), boost::bind(&Lobby::OnMessageSent, this));
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
	jngl::print(chatText_, -700, 350);
	DrawWidgets();
}

void Lobby::HandleReceive(std::string buf) {
	if (buf.length() > 0) {
		char actionType = buf[0];
		buf = buf.substr(1);
		switch(actionType) {
			case 'c':{
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
			}
			break;
		}
	}
	socket_->Receive(boost::bind(&Lobby::HandleReceive, this, _1));
}
