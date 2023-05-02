#include "menu.hpp"

#include "engine/paths.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "gui/ButtonBox.hpp"
#include "optionsmenu.hpp"
#include "engine/screen.hpp"
#include "Game.hpp"
#include "multiplayermenu.hpp"

#include <ctime>

Menu::Menu()
: buttonBox_(new ButtonBox(-450, 0)), normalHighscore_(GameType::NORMAL),
  fiftyLinesHighscore_(GameType::FIFTYLINES) {
	buttonBox_->add("Normal", std::bind(&Menu::Normal, this));
	buttonBox_->add("50 Lines", std::bind(&Menu::FiftyLines, this));
    buttonBox_->add("Multiplayer", []() {
		jngl::setWork(std::make_shared<Fade>(std::make_shared<MultiplayerMenu>()));
	});
	// buttonBox_->add("Multiplayer", []() {
	// 	jngl::setWork(std::make_shared<Fade>(std::make_shared<SplitScreen>(std::make_shared<GamepadControl>(1))));
	// });
	buttonBox_->add("Options", std::bind(&Menu::OptionsMenuCallback, this));
	buttonBox_->add("Quit", std::bind(&Menu::QuitGame, this));
	addWidget(buttonBox_);
}

void Menu::BlinkHighscore(Data data) {
	normalHighscore_.Blink(data);
	fiftyLinesHighscore_.Blink(data);
}

void Menu::step() {
	StepWidgets();
}

void Menu::draw() const {
	jngl::setBackgroundColor(0xffffff_rgb);
	DrawWidgets();
	GetScreen().DrawCentered("box", 340, 0);

	jngl::pushMatrix();
	jngl::translate(-20, -400);
	jngl::setFontSize(50);
	jngl::setFontColor(0, 0, 0);
	jngl::print("Highscores Normal", 0, -90);
	normalHighscore_.draw();
	jngl::translate(0, 530);
	jngl::setFontColor(0, 0, 0);
	jngl::print("Highscores 50 Lines", 0, -90);
	fiftyLinesHighscore_.draw();
	jngl::popMatrix();
}

void Menu::Normal() const {
	jngl::setWork(std::make_shared<Fade>(
	    std::make_shared<Game>(GameType::NORMAL, static_cast<int>(time(0)), false)));
}

void Menu::FiftyLines() const {
	jngl::setWork(std::make_shared<Fade>(
	    std::make_shared<Game>(GameType::FIFTYLINES, static_cast<int>(time(0)), false)));
}

void Menu::OptionsMenuCallback() const {
	jngl::setWork(std::make_shared<Fade>(std::make_shared<OptionsMenu>()));
}

void Menu::QuitGame() const {
	jngl::quit();
}
