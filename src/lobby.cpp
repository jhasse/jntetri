#ifndef WIZ
#include "lobby.hpp"
#include "engine/screen.hpp"
#include "engine/fade.hpp"

#include <boost/bind.hpp>
#include "engine/procedure.hpp"
#include "multiplayermenu.hpp"

#include <jngl.hpp>

Lobby::Lobby(boost::shared_ptr<Socket> socket)
	: socket_(socket), chatText_(""), input_(new Input(-700, 1100))
{
	logout_.reset(new Button("Logout", boost::bind(&Lobby::OnLogout, this)));
	play_.reset(new Button("Play!™", boost::bind(&Lobby::OnPlay, this)));
	HandleReceive("");
	AddWidget(input_);
	AddWidget(logout_);
	AddWidget(play_);
	logout_->CenterAt(-450, 150);
	play_->CenterAt(450, 150);
}

void Lobby::OnLogout()
{
	jngl::SetWork(new Fade(new MultiplayerMenu));
}

void Lobby::OnPlay()
{
	//waiting = true;
}

void Lobby::step()
{
	socket_->Step();
	if(jngl::KeyPressed(jngl::key::Return))
	{
		socket_->Send(std::string("c") + input_->GetText(), boost::bind(&Lobby::OnMessageSent, this));
		input_->SetSensitive(false);
	}
	StepWidgets();
}

void Lobby::OnMessageSent()
{
	input_->SetText("");
	input_->SetSensitive(true);
}

void Lobby::draw() const
{
	jngl::SetFontColor(0, 0, 0);
	jngl::SetFontSize(35);
	GetScreen().Print(chatText_, -700, 350);
	DrawWidgets();
}
#include <iostream>
void Lobby::HandleReceive(std::string buf)
{
	if(buf.length() > 0)
	{
		char actionType = buf[0];
		buf = buf.substr(1);
		switch(actionType)
		{
			case 'c':
			{
				chatText_ += buf;
				chatText_ += '\n';
				int lineCount = 0;
				size_t pos = 0;
				while((pos = chatText_.find_first_of("\n", pos + 1)) != std::string::npos)
				{
					++lineCount;
				}
				if(lineCount > 8)
				{
					pos = chatText_.find_first_of("\n");
					chatText_ = chatText_.substr(pos + 1);
				}
			}
			break;
		}
	}
	socket_->Receive(boost::bind(&Lobby::HandleReceive, this, _1));
}
#endif // WIZ
