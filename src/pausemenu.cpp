#include "pausemenu.hpp"

#include "Game.hpp"
#include "gui/ButtonBox.hpp"
#include "menu.hpp"

PauseMenu::PauseMenu(std::shared_ptr<Game> game)
: work(jngl::getWork()), buttonBox(new ButtonBox), game(std::move(game)) {
	buttonBox->add("Resume", [this] { Continue(); });
	buttonBox->add("Menu", [] { jngl::setScene<jngl::Fade>(std::make_shared<Menu>()); });
	if (jngl::canQuit()) {
		buttonBox->add("Quit", jngl::quit);
	}
	addWidget(buttonBox);
}

void PauseMenu::step() {
	if (jngl::keyPressed(jngl::key::Escape)) {
		jngl::setScene(work);
	}
	StepWidgets();
}

void PauseMenu::Continue() {
	jngl::setScene(work);
}

void PauseMenu::draw() const {
	game->draw();
	DrawWidgets();
}

void PauseMenu::onQuitEvent() {
	// Do Nothing
}
