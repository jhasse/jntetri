#include "multiplayermenu.hpp"
#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "menu.hpp"
#ifndef WIZ
#include "login.hpp"
#endif

#include <jngl.hpp>
#include <boost/bind.hpp>

MultiplayerMenu::MultiplayerMenu() : name_(new Input(50, 400)), password_(new Input(50, 500))
{
	AddWidget(name_);
	AddWidget(password_);
	login_.reset(new Button("Login", boost::bind(&MultiplayerMenu::OnLogin, this)));
	AddWidget(login_);
	back_.reset(new Button("Back", boost::bind(&MultiplayerMenu::OnBack, this)));
	AddWidget(back_);
	back_->CenterAt(-350, 880);
	login_->CenterAt(350, 880);
	password_->SetPassword(true);
	name_->SetText(GetOptions().Get<std::string>("lastLoginName"));
	if(name_->GetText() != "")
	{
		FocusNext();
	}
}

void MultiplayerMenu::step()
{
	StepWidgets();
	if(jngl::keyPressed(jngl::key::Return))
	{
		OnLogin();
	}
}

void MultiplayerMenu::draw() const
{
	jngl::setFontColor(0, 0, 0);
	GetScreen().Print("Name:", -500, 400);
	GetScreen().Print("Password:", -500, 500);
	DrawWidgets();
}

void MultiplayerMenu::OnBack() const
{
	jngl::setWork(new Fade(new Menu));
}

void MultiplayerMenu::OnLogin()
{
#ifndef WIZ
	jngl::setWork(new Login(boost::dynamic_pointer_cast<MultiplayerMenu>(jngl::getWork())));
#endif
}

std::string MultiplayerMenu::GetName() const
{
	return name_->GetText();
}

std::string MultiplayerMenu::GetPassword() const
{
	return password_->GetText();
}
