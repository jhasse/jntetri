#include "pausemenu.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "menu.hpp"
#include "engine/fade.hpp"

#include <jngl/all.hpp>

PauseMenu::PauseMenu(std::shared_ptr<jngl::Work> work)
: work_(std::dynamic_pointer_cast<Game>(work)), buttonBox_(new ButtonBox) {
	assert(work_);
	work_->SetRotateScreen(false); // Don't rotate the screen so that the buttons work correctly
	buttonBox_->add("Resume", std::bind(&PauseMenu::Continue, this));
	buttonBox_->add("Menu", std::bind(&PauseMenu::QuitToMenu, this));
	buttonBox_->add("Quit", jngl::quit);
	addWidget(buttonBox_);
}

void PauseMenu::step() {
	if (jngl::keyPressed(jngl::key::Escape)) {
		jngl::setWork(work_);
	}
	StepWidgets();
	work_->StepToRotateScreen();
}

void PauseMenu::Continue() {
	jngl::setWork(work_);
}

void PauseMenu::draw() const {
	work_->draw();
	DrawWidgets();
}

void PauseMenu::QuitToMenu() const {
	jngl::setWork(std::make_shared<Fade>(std::make_shared<Menu>()));
}

void PauseMenu::onQuitEvent() {
	// Do Nothing
}
