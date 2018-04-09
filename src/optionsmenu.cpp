#include "optionsmenu.hpp"
#include "menu.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "engine/screen.hpp"
#include "engine/options.hpp"

#include <jngl/all.hpp>

OptionsMenu::OptionsMenu() : startLevel_(new Chooser(0, -300)), startJunks_(new Chooser(0, 100)) {
	back_.reset(new Button("Back", std::bind(&OptionsMenu::OnBack, this)));
	for (int i = 0; i < 10; ++i) {
		startLevel_->AddValue(i);
	}
	for (unsigned int i = 0; i < getOptions().startLevel; ++i) {
		startLevel_->Next();
	}
	for (int i = 0; i < 6; ++i) {
		startJunks_->AddValue(i * 2);
	}
	for (unsigned int i = 0; i < getOptions().startJunks / 2; ++i) {
		startJunks_->Next();
	}
	back_->CenterAt(0, 400);
	addWidget(startLevel_);
	addWidget(startJunks_);
	addWidget(back_);
}

void OptionsMenu::step() {
	StepWidgets();
}

void OptionsMenu::draw() const {
	DrawWidgets();
	jngl::setFontColor(0, 0, 0);
	GetScreen().PrintCentered("Start Level", 0, -500);
	GetScreen().PrintCentered("Start Junks", 0, -100);
}

void OptionsMenu::OnBack() const {
	getOptions().startLevel = startLevel_->GetValue();
	getOptions().startJunks = startJunks_->GetValue();
	getOptions().Save();
	jngl::setWork(new Fade(new Menu));
}
