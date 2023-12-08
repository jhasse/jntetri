#include "multiplayermenu.hpp"

#include "engine/screen.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "menu.hpp"
#include "Login.hpp"

#include <jngl.hpp>

MultiplayerMenu::MultiplayerMenu(const bool quickLogin)
: name_(new Input(50, -240)), password_(new Input(50, -100)), quickLogin(quickLogin) {
	addWidget(name_);
	addWidget(password_);
	login_.reset(new Button("Login", [this]() { OnLogin(); }));
	addWidget(login_);
	back_.reset(new Button("Back", [this]() { OnBack(); }));
	addWidget(back_);
	back_->setCenter(-350, 280);
	login_->setCenter(350, 280);
	password_->SetPassword(true);
	name_->setText(getOptions().lastLoginName);
	if (name_->getText() != "") {
		FocusNext();
	}
}

void MultiplayerMenu::step() {
	StepWidgets();
	if (jngl::keyPressed(jngl::key::Return) || quickLogin) {
		quickLogin = false; // if an error happens we will be reactivated
		OnLogin();
	}
}

void MultiplayerMenu::draw() const {
	jngl::setFontColor(0, 0, 0);
	jngl::print("Name:", -500, -240);
	jngl::print("Password:", -500, -100);
	DrawWidgets();
}

void MultiplayerMenu::OnBack() const {
	jngl::setWork(std::make_shared<Fade>(std::make_shared<Menu>()));
}

void MultiplayerMenu::OnLogin() {
	jngl::setWork(
	    std::make_shared<Login>(std::dynamic_pointer_cast<MultiplayerMenu>(jngl::getWork())));
}

std::string MultiplayerMenu::GetName() const {
	return name_->getText();
}

std::string MultiplayerMenu::GetPassword() const {
	return password_->getText();
}

bool MultiplayerMenu::getQuickLogin() const {
	return quickLogin;
}
