#include "menu.hpp"
#include "engine/paths.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "optionsmenu.hpp"
#include "engine/screen.hpp"
#include "game.hpp"
#include "multiplayermenu.hpp"

#include <jngl/all.hpp>
#include <ctime>

Menu::Menu() : buttonBox_(new ButtonBox(-450, 0)), normalHighscore_(NORMAL), fiftyLinesHighscore_(FIFTYLINES) {
    buttonBox_->add("Normal", std::bind(&Menu::Normal, this));
	buttonBox_->add("50 Lines", std::bind(&Menu::FiftyLines, this));
    buttonBox_->add("Multiplayer", []() {
		jngl::setWork(new Fade(new MultiplayerMenu));
	});
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
	jngl::setBackgroundColor(255, 255, 255);
	DrawWidgets();
	GetScreen().DrawCentered("box", 340, 0);

	jngl::pushMatrix();
	jngl::translate(-20, -400);
	jngl::setFontSize(50);
	jngl::setFontColor(0, 0, 0);
	jngl::print("Highscores Normal", 0, -90);
	normalHighscore_.Draw();
	jngl::translate(0, 530);
	jngl::setFontColor(0, 0, 0);
	jngl::print("Highscores 50 Lines", 0, -90);
	fiftyLinesHighscore_.Draw();
	jngl::popMatrix();
}

void Menu::Normal() const {
	jngl::setWork(new Fade(new Game(NORMAL, static_cast<int>(time(0)))));
}

void Menu::FiftyLines() const {
	jngl::setWork(new Fade(new Game(FIFTYLINES, static_cast<int>(time(0)))));
}

void Menu::OptionsMenuCallback() const {
	jngl::setWork(new Fade(new OptionsMenu));
}

void Menu::QuitGame() const {
	jngl::quit();
}
