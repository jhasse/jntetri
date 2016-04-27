//ZUMYRDKTFFOVCNOVIRQDXXHWCQPUHPIMALEWEIVAGNXQJFKBFYZGMVFYXUJRFRCMFAFPMOTZFJZGHB
#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "menu.hpp"
#ifndef WIZ
#include "login.hpp"
#endif

#include <jngl/all.hpp>

MultiplayerMenu::MultiplayerMenu() : name_(new Input(50, -200)), password_(new Input(50, -100)) {
	addWidget(name_);
	addWidget(password_);
	login_.reset(new Button("Login", std::bind(&MultiplayerMenu::OnLogin, this)));
	addWidget(login_);
	back_.reset(new Button("Back", std::bind(&MultiplayerMenu::OnBack, this)));
	addWidget(back_);
	back_->CenterAt(-350, 280);
	login_->CenterAt(350, 280);
	password_->SetPassword(true);
	name_->SetText(GetOptions().Get<std::string>("lastLoginName"));
	if (name_->GetText() != "") {
		FocusNext();
	}
}

void MultiplayerMenu::step() {
	StepWidgets();
	if (jngl::keyPressed(jngl::key::Return)) {
		OnLogin();
	}
}

void MultiplayerMenu::draw() const {
	jngl::setFontColor(0, 0, 0);
	jngl::print("Name:", -500, -200);
	jngl::print("Password:", -500, -100);
	DrawWidgets();
}

void MultiplayerMenu::OnBack() const {
	jngl::setWork(new Fade(new Menu));
}

void MultiplayerMenu::OnLogin() {
	jngl::setWork(new Login(std::dynamic_pointer_cast<MultiplayerMenu>(jngl::getWork())));
}

std::string MultiplayerMenu::GetName() const {
	return name_->GetText();
}

std::string MultiplayerMenu::GetPassword() const {
	return password_->GetText();
}
