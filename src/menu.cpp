#include "menu.hpp"
#include "engine/paths.hpp"
#include "engine/procedure.hpp"
#include "engine/fade.hpp"
#include "optionsmenu.hpp"
#include "engine/screen.hpp"
#include "game.hpp"
#include "multiplayermenu.hpp"

#include <jngl.hpp>
#include <boost/bind.hpp>

Menu::Menu() : buttonBox_(-450, 600), normalHighscore_(NORMAL), fiftyLinesHighscore_(FIFTYLINES)
{
    buttonBox_.Add("Normal", boost::bind(&Menu::Normal, this), 'n');
	buttonBox_.Add("50 Lines", boost::bind(&Menu::FiftyLines, this), '5');
    buttonBox_.Add("Multiplayer", boost::bind(&Menu::Multiplayer, this), 'm');
	buttonBox_.Add("Options", boost::bind(&Menu::OptionsMenuCallback, this), 'o');
	buttonBox_.Add("Quit", boost::bind(&Menu::QuitGame, this), 'q');
}

void Menu::BlinkHighscore(Data data)
{
	normalHighscore_.Blink(data);
	fiftyLinesHighscore_.Blink(data);
}

void Menu::Step()
{
	buttonBox_.Step();
}

void Menu::Draw() const
{
	jngl::SetBackgroundColor(255, 255, 255);
	buttonBox_.Draw();
	GetScreen().DrawCentered("box.png", 340, 600);

	jngl::PushMatrix();
	GetScreen().Translate(-20, 200);
	GetScreen().SetFontSize(50);
	jngl::SetFontColor(0, 0, 0);
	GetScreen().Print("Highscores Normal", 0, -90);
	normalHighscore_.Draw();
	GetScreen().Translate(0, 530);
	jngl::SetFontColor(0, 0, 0);
	GetScreen().Print("Highscores 50 Lines", 0, -90);
	fiftyLinesHighscore_.Draw();
	jngl::PopMatrix();
}

void Menu::Normal() const
{
	Procedure::Handle().SetWork(new Fade(new Game(NORMAL)));
}

void Menu::FiftyLines() const
{
	Procedure::Handle().SetWork(new Fade(new Game(FIFTYLINES)));
}

void Menu::Multiplayer() const
{
	Procedure::Handle().SetWork(new Fade(new MultiplayerMenu()));
}

void Menu::OptionsMenuCallback() const
{
	Procedure::Handle().SetWork(new Fade(new OptionsMenu));
}

void Menu::QuitGame() const
{
    GetProcedure().Quit();
}
