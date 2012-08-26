#include "menu.hpp"
#include "engine/paths.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "optionsmenu.hpp"
#include "engine/screen.hpp"
#include "game.hpp"
#include "multiplayermenu.hpp"
#include "replayplayer.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>
#include <ctime>

Menu::Menu() : buttonBox_(new ButtonBox(-450, 0)), normalHighscore_(NORMAL), fiftyLinesHighscore_(FIFTYLINES)
{
    buttonBox_->Add("Normal", boost::bind(&Menu::Normal, this), 'n');
	buttonBox_->Add("50 Lines", boost::bind(&Menu::FiftyLines, this), '5');
    buttonBox_->Add("Multiplayer", boost::bind(&Menu::Multiplayer, this), 'm');
	buttonBox_->Add("Options", boost::bind(&Menu::OptionsMenuCallback, this), 'o');
	buttonBox_->Add("Quit", boost::bind(&Menu::QuitGame, this), 'q');
	AddWidget(buttonBox_);
}

void Menu::BlinkHighscore(Data data)
{
	normalHighscore_.Blink(data);
	fiftyLinesHighscore_.Blink(data);
}

void Menu::step()
{
	StepWidgets();
}

void Menu::draw() const
{
	jngl::setBackgroundColor(255, 255, 255);
	DrawWidgets();
	GetScreen().DrawCentered("box.png", 340, 0);

	jngl::pushMatrix();
	GetScreen().Translate(-20, -400);
	GetScreen().SetFontSize(50);
	jngl::setFontColor(0, 0, 0);
	GetScreen().Print("Highscores Normal", 0, -90);
	normalHighscore_.Draw();
	GetScreen().Translate(0, 530);
	jngl::setFontColor(0, 0, 0);
	GetScreen().Print("Highscores 50 Lines", 0, -90);
	fiftyLinesHighscore_.Draw();
	jngl::popMatrix();
}

void Menu::Normal() const
{
	jngl::setWork(new Fade(new Game(NORMAL, static_cast<int>(time(0)))));
}

void Menu::FiftyLines() const
{
	jngl::setWork(new Fade(new ReplayPlayer("test.jtr")));
}

void Menu::Multiplayer() const
{
	jngl::setWork(new Fade(new MultiplayerMenu()));
}

void Menu::OptionsMenuCallback() const
{
	jngl::setWork(new Fade(new OptionsMenu));
}

void Menu::QuitGame() const
{
	jngl::quit();
}
