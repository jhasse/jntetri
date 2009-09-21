#include "multiplayermenu.hpp"
#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "menu.hpp"
#include "login.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>

MultiplayerMenu::MultiplayerMenu()
	: back_(new Button("Back", boost::bind(&MultiplayerMenu::OnBack, this))),
	  login_(new Button("Login", boost::bind(&MultiplayerMenu::OnLogin, this))),
	  name_(new Input(50, 400)), password_(new Input(50, 500))
{
	AddWidget(name_);
	AddWidget(password_);
	AddWidget(login_);
	AddWidget(back_);
	back_->CenterAt(-350, 880);
	login_->CenterAt(350, 880);
	password_->SetPassword(true);
	name_->SetText(GetOptions().GetLastLoginName());
	if(name_->GetText() != "")
	{
		FocusNext();
	}
}

void MultiplayerMenu::Step()
{
	StepWidgets();
	if(jngl::KeyPressed(jngl::key::Return))
	{
		OnLogin();
	}
}

void MultiplayerMenu::Draw() const
{
	jngl::SetFontColor(0, 0, 0);
	GetScreen().Print("Name:", -500, 400);
	GetScreen().Print("Password:", -500, 500);
	DrawWidgets();
}

void MultiplayerMenu::OnBack() const
{
	GetProcedure().SetWork(new Fade(new Menu));
}

void MultiplayerMenu::OnLogin()
{
	GetProcedure().SetWork(new Login(boost::dynamic_pointer_cast<MultiplayerMenu>(GetProcedure().GetWork())));
}

std::string MultiplayerMenu::GetName() const
{
	return name_->GetText();
}

std::string MultiplayerMenu::GetPassword() const
{
	return password_->GetText();
}
