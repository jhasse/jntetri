#include "pausemenu.hpp"

#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "engine/fade.hpp"

#include <jngl/all.hpp>

PauseMenu::PauseMenu(std::shared_ptr<Game> game)
: work(jngl::getWork()), buttonBox_(new ButtonBox), game(std::move(game)) {
	this->game->SetRotateScreen(false); // Don't rotate the screen so that the buttons work correctly
	buttonBox_->add("Resume", std::bind(&PauseMenu::Continue, this));
	buttonBox_->add("Menu", std::bind(&PauseMenu::QuitToMenu, this));
	buttonBox_->add("Quit", jngl::quit);
	addWidget(buttonBox_);
}

void PauseMenu::step() {
	if (jngl::keyPressed(jngl::key::Escape)) {
		jngl::setWork(work);
	}
	StepWidgets();
	game->StepToRotateScreen();
}

void PauseMenu::Continue() {
	jngl::setWork(work);
}

void PauseMenu::draw() const {
	game->draw();
	DrawWidgets();
}

void PauseMenu::QuitToMenu() const {
	jngl::setWork(std::make_shared<Fade>(std::make_shared<Menu>()));
}

void PauseMenu::onQuitEvent() {
	// Do Nothing
}
