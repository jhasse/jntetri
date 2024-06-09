#include "pausemenu.hpp"

#include "Game.hpp"
#include "engine/fade.hpp"
#include "gui/ButtonBox.hpp"
#include "menu.hpp"

PauseMenu::PauseMenu(std::shared_ptr<Game> game)
: work(jngl::getWork()), buttonBox(new ButtonBox), game(std::move(game)) {
	this->game->SetRotateScreen(false); // Don't rotate the screen so that the buttons work correctly
	buttonBox->add("Resume", std::bind(&PauseMenu::Continue, this));
	buttonBox->add("Menu", std::bind(&PauseMenu::QuitToMenu, this));
	if (jngl::canQuit()) {
		buttonBox->add("Quit", jngl::quit);
	}
	addWidget(buttonBox);
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
